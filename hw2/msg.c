#include<windows.h>
#include<stdio.h>
__declspec(dllexport)
//注释掉上一行运行时会出错，修正：在链接时加上 /export:showmsg
 int showmsg(char *s)
{
	  HMODULE i=GetModuleHandle(0);
	  int str[100];
	  sprintf(str,"base:0x%08x",i);
	  //讲程序的加载地址写入str数组
	  if (MessageBoxA(0, s, str, MB_YESNO) == IDYES)
			 return 1;
      else 
      	     return 0;
}