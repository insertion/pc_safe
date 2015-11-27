int main()
{
	__asm
	{
		jmp calls
	pops:
		mov eax,0xaabbaabb
		call eax
		ret	
	calls:
		call pops
	}	;
}