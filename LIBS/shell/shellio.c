#include "shellio.h"

unsigned char Str_Compare(char * str1, char * str2, unsigned short int len)
{
	for(unsigned short int i = 0; i < len; i++)
	{
		if(str1[i] != str2[i])
		{
			return 0;
		}
	}
	return 1;
}

float String_To_Float(char *array)
{
	uint8_t len = 0;
	uint8_t point_num = 0;
	short f1 = 0;
	float f2 = 0;
	while(array[len])
	{
		if(array[len] == '.')
		{
			point_num = len;
		}
		len++;
	}
	for(u8 i=0; i<point_num; i++)
	{
		f1 *= 10;
		f1 += (array[i] - '0');
	}
	for(u8 i=len-1; i>point_num; i--)
	{
		f2 += (array[i] - '0');
		f2 /= 10;
	}
	return ( f1 + f2 );
}
