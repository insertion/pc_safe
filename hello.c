#include<stdio.h>
extern my_print(char *);
int main( int argc,char **argv)
{
	int addr;
	void (*test)(char *); 
	//test=0x40100a;//函数指针的值为我的电脑打印的my_print函数的地址
    
	my_print("hello\n");
	__asm
	{
		    mov eax,[esp-4]
		    mov dword ptr [addr],eax
	};
	printf("hello的地址是： 0x%x\n",(int)addr);
	printf("我们可以输出这个地址的内容： %s\n",(char *)addr);
	printf("mian:    0x%x\nprintf:  0x%x\nmy_print:0x%x\n",(int)main,(int)printf,(int)my_print);
	
	//test("\n0x40100a 确实是my_print的地址\n");//测试我们输出的函数地址是否正确
	while (1);
	return 0;
}