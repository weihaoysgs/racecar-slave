#include "rtt_cmd.h"
#include "shell.h"
#include "shellio.h"
#include "string.h"
extern Shell_command_t shell_cmd_root;
#define rtt_print shell_print

void rtt_cmd_help(char * arg)
{
    rtt_print("RT-Thread shell commands:\r\n");
    rtt_print("version         - show RT-Thread version information\r\n");
    rtt_print("list_thread     - list thread\r\n");
    rtt_print("list_sem        - list semaphore in system\r\n");
    rtt_print("list_event      - list event in system\r\n");
    rtt_print("list_mutex      - list mutex in system\r\n");
    rtt_print("list_mailbox    - list mail box in system\r\n");
    rtt_print("list_msgqueue   - list message queue in system\r\n");
    rtt_print("list_timer      - list timer in system\r\n");
    rtt_print("list_device     - list device in system\r\n");
    rtt_print("exit            - return to RT-Thread shell mode.\r\n");
    rtt_print("help            - RT-Thread shell help.\r\n");
    rtt_print("ps              - List threads in the system.\r\n");
    rtt_print("time            - Execute command with time.\r\n");
    rtt_print("free            - Show the memory usage in the system.\r\n");
}

//来自RTT FinSH组件内置命令

#include <rthw.h>
#include <rtthread.h>

#define LIST_FIND_OBJ_NR 8

rt_inline void object_split(int len)
{
    while (len--) rtt_print("-");
}

typedef struct
{
    rt_list_t *list;
    rt_list_t **array;
    rt_uint8_t type;
    int nr;             /* input: max nr, can't be 0 */
    int nr_out;         /* out: got nr */
} list_get_next_t;

static void list_find_init(list_get_next_t *p, rt_uint8_t type, rt_list_t **array, int nr)
{
    struct rt_object_information *info;
    rt_list_t *list;

    info = rt_object_get_information((enum rt_object_class_type)type);
    list = &info->object_list;

    p->list = list;
    p->type = type;
    p->array = array;
    p->nr = nr;
    p->nr_out = 0;
}

static rt_list_t *list_get_next(rt_list_t *current, list_get_next_t *arg)
{
    int first_flag = 0;
    rt_ubase_t level;
    rt_list_t *node, *list;
    rt_list_t **array;
    int nr;

    arg->nr_out = 0;

    if (!arg->nr || !arg->type)
    {
        return (rt_list_t *)RT_NULL;
    }

    list = arg->list;

    if (!current) /* find first */
    {
        node = list;
        first_flag = 1;
    }
    else
    {
        node = current;
    }

    level = rt_hw_interrupt_disable();

    if (!first_flag)
    {
        struct rt_object *obj;
        /* The node in the list? */
        obj = rt_list_entry(node, struct rt_object, list);
        if ((obj->type & ~RT_Object_Class_Static) != arg->type)
        {
            rt_hw_interrupt_enable(level);
            return (rt_list_t *)RT_NULL;
        }
    }

    nr = 0;
    array = arg->array;
    while (1)
    {
        node = node->next;

        if (node == list)
        {
            node = (rt_list_t *)RT_NULL;
            break;
        }
        nr++;
        *array++ = node;
        if (nr == arg->nr)
        {
            break;
        }
    }
    
    rt_hw_interrupt_enable(level);
    arg->nr_out = nr;
    return node;
}

//列出所有线程
void list_thread(char * arg)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;
    const char *item_title = "thread";
    int maxlen;

    list_find_init(&find_arg, RT_Object_Class_Thread, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

#ifdef RT_USING_SMP
    rtt_print("%-*.s cpu pri  status      sp     stack size max used left tick  error\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(     " --- ---  ------- ---------- ----------  ------  ---------- ---\r\n");
#else
    rtt_print("%-*.s pri  status      sp     stack size max used left tick  error\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(     " ---  ------- ---------- ----------  ------  ---------- ---\r\n");
#endif /*RT_USING_SMP*/

    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_thread thread_info, *thread;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();

                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }
                /* copy info */
                memcpy(&thread_info, obj, sizeof thread_info);
                rt_hw_interrupt_enable(level);

                thread = (struct rt_thread*)obj;
                {
                    rt_uint8_t stat;
                    rt_uint8_t *ptr;

#ifdef RT_USING_SMP
                    if (thread->oncpu != RT_CPU_DETACHED)
                        rtt_print("%-*.*s %3d %3d ", maxlen, RT_NAME_MAX, thread->name, thread->oncpu, thread->current_priority);
                    else
                        rtt_print("%-*.*s N/A %3d ", maxlen, RT_NAME_MAX, thread->name, thread->current_priority);

#else
                    rtt_print("%-*.*s %3d ", maxlen, RT_NAME_MAX, thread->name, thread->current_priority);
#endif /*RT_USING_SMP*/
                    stat = (thread->stat & RT_THREAD_STAT_MASK);
                    if (stat == RT_THREAD_READY)        rtt_print(" ready  ");
                    else if (stat == RT_THREAD_SUSPEND) rtt_print(" suspend");
                    else if (stat == RT_THREAD_INIT)    rtt_print(" init   ");
                    else if (stat == RT_THREAD_CLOSE)   rtt_print(" close  ");
                    else if (stat == RT_THREAD_RUNNING) rtt_print(" running");

#if defined(ARCH_CPU_STACK_GROWS_UPWARD)
                    ptr = (rt_uint8_t *)thread->stack_addr + thread->stack_size - 1;
                    while (*ptr == '#')ptr --;

                    rtt_print(" 0x%08x 0x%08x    %02d%%   0x%08x %03d\r\n",
                            ((rt_ubase_t)thread->sp - (rt_ubase_t)thread->stack_addr),
                            thread->stack_size,
                            ((rt_ubase_t)ptr - (rt_ubase_t)thread->stack_addr) * 100 / thread->stack_size,
                            thread->remaining_tick,
                            thread->error);
#else
                    ptr = (rt_uint8_t *)thread->stack_addr;
                    while (*ptr == '#')ptr ++;

                    rtt_print(" 0x%08x 0x%08x    %02d%%   0x%08x %03d\r\n",
                            thread->stack_size + ((rt_ubase_t)thread->stack_addr - (rt_ubase_t)thread->sp),
                            thread->stack_size,
                            (thread->stack_size - ((rt_ubase_t) ptr - (rt_ubase_t) thread->stack_addr)) * 100
                            / thread->stack_size,
                            thread->remaining_tick,
                            thread->error);
#endif
                }
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

}

static void show_wait_queue(struct rt_list_node *list)
{
    struct rt_thread *thread;
    struct rt_list_node *node;

    for (node = list->next; node != list; node = node->next)
    {
        thread = rt_list_entry(node, struct rt_thread, tlist);
        rtt_print("%s", thread->name);

        if (node->next != list)
            rtt_print("/");
    }
}

#ifdef RT_USING_SEMAPHORE
void list_sem(char * arg)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "semaphore";

    list_find_init(&find_arg, RT_Object_Class_Semaphore, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rtt_print("%-*.s v   suspend thread\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(     " --- --------------\r\n");

    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_semaphore *sem;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }
                rt_hw_interrupt_enable(level);

                sem = (struct rt_semaphore*)obj;
                if (!rt_list_isempty(&sem->parent.suspend_thread))
                {
                    rtt_print("%-*.*s %03d %d:",
                            maxlen, RT_NAME_MAX,
                            sem->parent.parent.name,
                            sem->value,
                            rt_list_len(&sem->parent.suspend_thread));
                    show_wait_queue(&(sem->parent.suspend_thread));
                    rtt_print("\r\n");
                }
                else
                {
                    rtt_print("%-*.*s %03d %d\r\n",
                            maxlen, RT_NAME_MAX,
                            sem->parent.parent.name,
                            sem->value,
                            rt_list_len(&sem->parent.suspend_thread));
                }
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

}
#endif

#ifdef RT_USING_EVENT
void list_event(char * arg)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "event";

    list_find_init(&find_arg, RT_Object_Class_Event, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rtt_print("%-*.s      set    suspend thread\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(     "  ---------- --------------\r\n");

    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_event *e;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                e = (struct rt_event *)obj;
                if (!rt_list_isempty(&e->parent.suspend_thread))
                {
                    rtt_print("%-*.*s  0x%08x %03d:",
                            maxlen, RT_NAME_MAX,
                            e->parent.parent.name,
                            e->set,
                            rt_list_len(&e->parent.suspend_thread));
                    show_wait_queue(&(e->parent.suspend_thread));
                    rtt_print("\r\n");
                }
                else
                {
                    rtt_print("%-*.*s  0x%08x 0\r\n",
                            maxlen, RT_NAME_MAX, e->parent.parent.name, e->set);
                }
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

}
#endif

#ifdef RT_USING_MUTEX
void list_mutex(char * arg)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "mutex";

    list_find_init(&find_arg, RT_Object_Class_Mutex, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rtt_print("%-*.s   owner  hold suspend thread\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(     " -------- ---- --------------\r\n");

    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_mutex *m;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                m = (struct rt_mutex *)obj;
                rtt_print("%-*.*s %-8.*s %04d %d\r\n",
                        maxlen, RT_NAME_MAX,
                        m->parent.parent.name,
                        RT_NAME_MAX,
                        m->owner->name,
                        m->hold,
                        rt_list_len(&m->parent.suspend_thread));

            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

}
#endif

#ifdef RT_USING_MAILBOX
void list_mailbox(char * arg)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "mailbox";

    list_find_init(&find_arg, RT_Object_Class_MailBox, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rtt_print("%-*.s entry size suspend thread\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(     " ----  ---- --------------\r\n");

    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_mailbox *m;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                m = (struct rt_mailbox *)obj;
                if (!rt_list_isempty(&m->parent.suspend_thread))
                {
                    rtt_print("%-*.*s %04d  %04d %d:",
                            maxlen, RT_NAME_MAX,
                            m->parent.parent.name,
                            m->entry,
                            m->size,
                            rt_list_len(&m->parent.suspend_thread));
                    show_wait_queue(&(m->parent.suspend_thread));
                    rtt_print("\r\n");
                }
                else
                {
                    rtt_print("%-*.*s %04d  %04d %d\r\n",
                            maxlen, RT_NAME_MAX,
                            m->parent.parent.name,
                            m->entry,
                            m->size,
                            rt_list_len(&m->parent.suspend_thread));
                }

            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

}
#endif

#ifdef RT_USING_MESSAGEQUEUE
void list_msgqueue(char * arg)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "msgqueue";

    list_find_init(&find_arg, RT_Object_Class_MessageQueue, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rtt_print("%-*.s entry suspend thread\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(     " ----  --------------\r\n");
    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_messagequeue *m;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                m = (struct rt_messagequeue *)obj;
                if (!rt_list_isempty(&m->parent.suspend_thread))
                {
                    rtt_print("%-*.*s %04d  %d:",
                            maxlen, RT_NAME_MAX,
                            m->parent.parent.name,
                            m->entry,
                            rt_list_len(&m->parent.suspend_thread));
                    show_wait_queue(&(m->parent.suspend_thread));
                    rtt_print("\r\n");
                }
                else
                {
                    rtt_print("%-*.*s %04d  %d\r\n",
                            maxlen, RT_NAME_MAX,
                            m->parent.parent.name,
                            m->entry,
                            rt_list_len(&m->parent.suspend_thread));
                }
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

}
#endif

#ifdef RT_USING_MEMHEAP
void list_memheap(char * arg)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "memheap";

    list_find_init(&find_arg, RT_Object_Class_MemHeap, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rtt_print("%-*.s  pool size  max used size available size\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(      " ---------- ------------- --------------\r\n");
    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_memheap *mh;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                mh = (struct rt_memheap *)obj;

                rtt_print("%-*.*s %-010d %-013d %-05d\r\n",
                        maxlen, RT_NAME_MAX,
                        mh->parent.name,
                        mh->pool_size,
                        mh->max_used_size,
                        mh->available_size);

            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

}
#endif

#ifdef RT_USING_MEMPOOL
void list_mempool(char * arg)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "mempool";

    list_find_init(&find_arg, RT_Object_Class_MemPool, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rtt_print("%-*.s block total free suspend thread\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(     " ----  ----  ---- --------------\r\n");
    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_mempool *mp;
                int suspend_thread_count;
                rt_list_t *node;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                mp = (struct rt_mempool *)obj;

                suspend_thread_count = 0;
                rt_list_for_each(node, &mp->suspend_thread)
                {
                    suspend_thread_count++;
                }

                if (suspend_thread_count > 0)
                {
                    rtt_print("%-*.*s %04d  %04d  %04d %d:",
                            maxlen, RT_NAME_MAX,
                            mp->parent.name,
                            mp->block_size,
                            mp->block_total_count,
                            mp->block_free_count,
                            suspend_thread_count);
                    show_wait_queue(&(mp->suspend_thread));
                    rtt_print("\r\n");
                }
                else
                {
                    rtt_print("%-*.*s %04d  %04d  %04d %d\r\n",
                            maxlen, RT_NAME_MAX,
                            mp->parent.name,
                            mp->block_size,
                            mp->block_total_count,
                            mp->block_free_count,
                            suspend_thread_count);
                }
            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

}
#endif

void list_timer(char * arg)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "timer";

    list_find_init(&find_arg, RT_Object_Class_Timer, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rtt_print("%-*.s  periodic   timeout       flag\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(     " ---------- ---------- -----------\r\n");
    do {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_timer *timer;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                timer = (struct rt_timer *)obj;
                rtt_print("%-*.*s 0x%08x 0x%08x ",
                        maxlen, RT_NAME_MAX,
                        timer->parent.name,
                        timer->init_tick,
                        timer->timeout_tick);
                if (timer->parent.flag & RT_TIMER_FLAG_ACTIVATED)
                    rtt_print("activated\r\n");
                else
                    rtt_print("deactivated\r\n");

            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

    rtt_print("current tick:0x%08x\r\n", rt_tick_get());

}

#ifdef RT_USING_DEVICE
static char *const device_type_str[] =
{
    "Character Device",
    "Block Device",
    "Network Interface",
    "MTD Device",
    "CAN Device",
    "RTC",
    "Sound Device",
    "Graphic Device",
    "I2C Bus",
    "USB Slave Device",
    "USB Host Bus",
    "SPI Bus",
    "SPI Device",
    "SDIO Bus",
    "PM Pseudo Device",
    "Pipe",
    "Portal Device",
    "Timer Device",
    "Miscellaneous Device",
    "Sensor Device",
    "Touch Device",
    "Unknown"
};

void list_device(char * arg)
{
    rt_ubase_t level;
    list_get_next_t find_arg;
    rt_list_t *obj_list[LIST_FIND_OBJ_NR];
    rt_list_t *next = (rt_list_t*)RT_NULL;

    int maxlen;
    const char *item_title = "device";

    list_find_init(&find_arg, RT_Object_Class_Device, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

    maxlen = RT_NAME_MAX;

    rtt_print("%-*.s         type         ref count\r\n", maxlen, item_title); object_split(maxlen);
    rtt_print(     " -------------------- ----------\r\n");
    do
    {
        next = list_get_next(next, &find_arg);
        {
            int i;
            for (i = 0; i < find_arg.nr_out; i++)
            {
                struct rt_object *obj;
                struct rt_device *device;

                obj = rt_list_entry(obj_list[i], struct rt_object, list);
                level = rt_hw_interrupt_disable();
                if ((obj->type & ~RT_Object_Class_Static) != find_arg.type)
                {
                    rt_hw_interrupt_enable(level);
                    continue;
                }

                rt_hw_interrupt_enable(level);

                device = (struct rt_device *)obj;
                rtt_print("%-*.*s %-20s %-8d\r\n",
                        maxlen, RT_NAME_MAX,
                        device->parent.name,
                        (device->type <= RT_Device_Class_Unknown) ?
                        device_type_str[device->type] :
                        device_type_str[RT_Device_Class_Unknown],
                        device->ref_count);

            }
        }
    }
    while (next != (rt_list_t*)RT_NULL);

}
#endif

//RTT命令初始化
void Rtt_Cmd_Init(void)
{
    Shell_Register_Command("rtt-cmd-help" , rtt_cmd_help);
    Shell_Register_Command("rtt-list-thread" , list_thread);

#ifdef RT_USING_SEMAPHORE
    Shell_Register_Command("rtt-list-sem" , list_sem);
#endif

#ifdef RT_USING_EVENT
    Shell_Register_Command("rtt-list-event" , list_event);
#endif

#ifdef RT_USING_MUTEX
    Shell_Register_Command("rtt-list-mutex" , list_mutex);
#endif

#ifdef RT_USING_MAILBOX
    Shell_Register_Command("rtt-list-mailbox" , list_mailbox);
#endif

#ifdef RT_USING_MESSAGEQUEUE
    Shell_Register_Command("rtt-list-msgqueue" , list_msgqueue);
#endif

#ifdef RT_USING_MEMHEAP
    Shell_Register_Command("rtt-list-memheap" , list_memheap);
#endif

#ifdef RT_USING_MEMPOOL
    Shell_Register_Command("rtt-list-mempool" , list_mempool);  
#endif

Shell_Register_Command("rtt-list-timer" , list_timer);

#ifdef RT_USING_DEVICE
    Shell_Register_Command("rtt-listdevice" , list_device);
#endif

}
