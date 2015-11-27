#include <windows.h>
#include<stdio.h>

__declspec(dllimport) int ShowMsg(char *s);

char *s = "Hello world!";
int i=0;
char buf[100];


int WINAPI WinMain(HINSTANCE hInstance, 
                   HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, 
                   int nCmdShow)
{
    int pid = 0;
    pid = GetCurrentProcessId();
    while (ShowMsg(buf)){
     sprintf(buf, "base=0x%08x\npid=%d\n%s", 
            hInstance, pid, s);
        i++;
    }
    return 0;
}