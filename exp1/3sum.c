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