#include "delay.h" 

void delay_ms(unsigned int ms_count)           //1ms
{
	unsigned int i=0;
	for(; i<ms_count; i++)
	{
		unsigned short nCount=10250;                   //ʹ���߼������ǲ��
		for(; nCount != 0; nCount--);
	}
}

void delay_us(unsigned int us_count)
{
	unsigned int i=0;
	for(; i<us_count; i++)
	{
		unsigned char nCount=9;            //ʹ���߼������ǲ��
		for(; nCount != 0; nCount--);
	}	
}



