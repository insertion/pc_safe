
#include<stdio.h>
int fib(int n)
{
	__asm
	{
		mov ecx,[n]
		sub ecx,1
		mov ebx,0
		mov eax,1
  floop:
  		mov edx,eax
  		add eax,ebx
		mov ebx,edx
		loop floop
		
	}
	//return 0;
}
int main()
{
	printf("fib(20) = %u",fib(20));
	return 0;
}