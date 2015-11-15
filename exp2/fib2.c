#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int n, r;
    if (argc < 2) {
        printf("Usage: %s number(<40)\n", argv[0]);
        return -1;
    }
    n = atoi(argv[1]);
    __asm 
	{
		push [n]
		call fib2
    mov [r],eax
		jmp out
		
    
    fib2:
      push ebp
      mov ebp,esp
      push esi
      
		  cmp [ebp+8],1
		  jz retn1
		  cmp [ebp+8],0
		  jz retn0

		  
      mov ecx,[ebp+8]
      
		  sub ecx,1
		  push ecx
		  call fib2
		  mov esi,eax
      add esp,4
      
      mov ecx,[ebp+8]
      sub ecx,2
      push ecx
      call fib2 
		  add esp,4
		 
      add eax,esi
      jmp end
retn1:
		  mov eax,1
		  jmp end
retn0:
		  mov eax,0
		  jmp end
     
  end:
      pop esi
      pop ebp
		  ret
	}
  out: printf("fib(%d) = %d\n", n, r);
    return 0;
}