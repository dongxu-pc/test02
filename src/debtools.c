
#include "debtools.h"

inline void hexcase(int c);

void printXstring(const unsigned char* str,size_t n)
{
	size_t i;
	for(i = 0;i < n;i++){
		printXchar(str[i]);
	}
	printf("\n");
}

inline void printXchar(unsigned char a)
{
	hexcase(a >> 4);
	hexcase(a%16);
	printf(" ");
}

inline void hexcase(int c)
{
	switch(c){
		case 10: printf("A");break;
		case 11: printf("B");break;
		case 12: printf("C");break;
		case 13: printf("D");break;
		case 14: printf("E");break;
		case 15: printf("F");break;
		default:printf("%d",c);break;
	}
}