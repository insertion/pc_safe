#include <stdio.h>

struct ctx {
  int eip, esp, ebx, ebp;
} M, A;

__declspec(naked) void swtch(struct ctx *from, struct ctx *to)
{
    __asm{
        mov eax, [esp+4]//esp+4是第一个参数from的地址
        pop dword ptr [eax]
        //把返回地址保存在from的eip中
        mov [eax+4], esp
        mov [eax+8], ebx
        mov [eax+12], ebp
        //把各寄存器值保存在from的对应寄存器中
        
        mov ecx, [esp+4] //此时的esp+4存放的是to的EIP
        mov ebp, [ecx+12]
        mov ebx, [ecx+8]
        mov esp, [ecx+4]
        push [ecx]
        //重新设定返回地址，返回地址为A的EIP
        ret
    }
}

void taskA()
{
    printf("A: 1\n");
    swtch(&A, &M);
    printf("A: 2\n");
    swtch(&A, &M);
    printf("A: 3\n");
    swtch(&A, &M);
    printf("A: 4\n");
    swtch(&A, &M);
    printf("A: 5\n");
    swtch(&A, &M);
    printf("A: 6\n");
    swtch(&A, &M);
    printf("A: 7\n");
    swtch(&A, &M);
}

int main()
{
  int stackA[1024];
  
  A.eip = (int)taskA;
  A.esp = (int)(&stackA[1023]);
  swtch(&M, &A);
  printf("M:       1\n");
  swtch(&M, &A);
  printf("M:       2\n");
  swtch(&M, &A);
  printf("M:       3\n");
  swtch(&M, &A);
  printf("M:       4\n");
  swtch(&M, &A);
  printf("M:       5\n");
  swtch(&M, &A);
  printf("M:       6\n");
  return 0;
}