#include<windows.h>
int * entryPoint;
int op;
int main()
{
    int op;
    VirtualProtect(entryPoint,4,PAGE_EXECUTE_READWRITE,&op);
}