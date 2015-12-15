#include <windows.h>
#include<stdio.h>
#pragma comment(lib, "user32.lib")
int main()
{
    char buff[20];
    int pid = GetCurrentProcessId();
    printf("[MessageBoxA]:pid is %d\n",pid);
    while (MessageBoxA(0,"happy", "hello.exe", MB_YESNO) == IDYES) {}
    return 0;
}

