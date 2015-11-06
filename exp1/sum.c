#include<stdio.h>
#include<stdlib.h>
int sum( int i)
{
	__asm
	{
		mov ecx,[ebp+8]
		mov eax,0
	loop_add:
		add eax,ecx
		loop loop_add
	}
}
int main(int argc,char **argv)
{
	int i;
	if(argc>1)
	i = atoi(argv[1]);
	printf("sum(100) = %d",sum(i));
}
