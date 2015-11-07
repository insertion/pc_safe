#include<stdio.h>
int add(int x,int y)
{
	__asm
	{
		mov eax,[ebp+8]
		add eax,[ebp+12]
	}
}

int main()
{
	int i=10;
	i=add(2,4);
	printf("add(2,4) = %d\n",i);
}

//程序被加载到内存中的基地址:0x00401548
//mian函数的第一条指令为：push ebp 其地址为：00401555