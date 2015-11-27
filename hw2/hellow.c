#include<windows.h>
#include<stdio.h>
//__declspec(dllimport) 
//注释掉上一行并不会出错
int showmsg(char *);
char *s="hello world!";
int i=0;
char buf[100];
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, 
                   int nCmdShow)
{
    int pid = 0;
    pid = GetCurrentProcessId();
    while (showmsg(buf))
	{
        sprintf(buf, "base=0x%08x\n pid=%d\n %s", 
           (unsigned int) hInstance, &i, s);
        i++;
       // printf("i 的地址%p",&i);
    }
    return 0;				   
}