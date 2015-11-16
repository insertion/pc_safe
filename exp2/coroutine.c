#include <stdio.h>

struct ctx {
  int eip, esp, ebx, ebp;
} M, A, B;

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
  int i;
  for(i=1;i<=9;i++)
  {
     printf("A: %d\n",i);
      swtch(&A, &B);
  }
  printf("A: %d\n",i);
  swtch(&A,&M);
}
void taskB()
{
   int i;
  for(i=1;i<=9;i++)
  {
     printf("B: %d\n",i);
     swtch(&B, &M);
  }
}
int main()
{
  int stackA[1024];
  int stackB[1024];
  A.eip = (int)taskA;
  B.eip = (int)taskB;
  A.esp = (int)(&stackA[1023]);
  B.esp = (int)(&stackB[1023]);
  
  
   int i;
  for(i=1;i<=8;i++)
  {
     swtch(&M, &A);
     printf("C: %d\n\n",i);
  }
  swtch(&M, &A);
  swtch(&M, &A);
  return 0;
}