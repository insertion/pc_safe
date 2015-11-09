#include<stdio.h>
#include<stdlib.h>
extern int sum(int);
int main(int argc,char **argv)
{	
	char string[]= "sum = %d";
	int i=100;
	if(argc>1)
	i = atoi(argv[1]);

	__asm
	{
		push [i]
		call sum
		add esp,4
		
		push eax
		lea eax,[string]
		push  eax
		call DWORD PTR printf
		add  esp,8

	
	}
	return 0;
}
//asm zhong buneng yong precode
//	printf("sum(100) = %d",sum(i));

	//用 _emit 定义字节。如：
//_asm _emit 0x90
//或
//_asm
//{
//    _emit 0x90
//}