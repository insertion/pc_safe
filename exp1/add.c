#include<stdio.h>
int add(int x,int y)
{
	__asm
	{
		mov eax,[ebp+12]
		mov ebx,[ebp+8]
		add eax,ebx
		//ebp+4是返回地址，ebp+8是第一个参数，ebp+8是第二个参数
		//h函数的返回值放在eax中
	}
}

int main()
{
	printf("add(2,4) = %d\n",add(2,4));
}

//程序被加载到内存中的基地址:0x00401548
//mian函数的第一条指令为：push ebp 其地址为：00401555