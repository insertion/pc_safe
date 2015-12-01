#include<windows.h>

void main()
{
	__asm
	{
		jmp str1
	loadl:
		mov eax,0xaabbaabb
		call eax
	
	getpross:
		jmp str2
		push eax
		mov eax,0xaabbaabb
		call eax
	
		push 0
		jmp str3
	message:
		pop ebx
		push ebx
		push ebx
		push 0
		call eax
		ret	
	str1:
		call loadl
		nop
	str2:
		call getpross
		nop
	str3:
		call message
		nop
	};
}