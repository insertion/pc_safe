#include<stdio.h>
extern my_print(char *);
int main( int argc,char **argv)
{
	int addr;
	void (*test)(char *); 
	//test=0x40100a;//����ָ���ֵΪ�ҵĵ��Դ�ӡ��my_print�����ĵ�ַ
    
	my_print("hello\n");
	__asm
	{
		    mov eax,[esp-4]
		    mov dword ptr [addr],eax
	};
	printf("hello�ĵ�ַ�ǣ� 0x%x\n",(int)addr);
	printf("���ǿ�����������ַ�����ݣ� %s\n",(char *)addr);
	printf("mian:    0x%x\nprintf:  0x%x\nmy_print:0x%x\n",(int)main,(int)printf,(int)my_print);
	
	//test("\n0x40100a ȷʵ��my_print�ĵ�ַ\n");//������������ĺ�����ַ�Ƿ���ȷ
	while (1);
	return 0;
}