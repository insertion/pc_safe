#include <windows.h>
#include<stdio.h>
#include<string.h>
//int i;
BYTE code[]={0x6a,0x00,0xb8,
    0xbb,0xaa,0xbb,0xaa, //0xAABBAABB
    0xff,0xd0,0xc3};
int main(int argc, char *argv[])
{
    int pid = 0;
    int exitcode=0;
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
//ReadProcessMemory(hproc, 0x004177E0, &i, sizeof(i), 0);
//WriteProcessMemory(hproc,0x417004,s,strlen(s),0);
//printf("> i = : [%d]\n",i);
//od执行hellow.exe时，该程序无法读取i的数据
// FILL YOUR CODE HERE


/**************************************作业3************************************/
addr=(int)GetModuleHandleA;
printf("GetModuleHandleA is 0x%x\n",addr);
//重新填写getmoudlehandle的地址
code[3]= (addr)     &255;
code[4]= (addr>>8)  &255;
code[5]= (addr>>16) &255;
code[6]= (addr>>24) &255;
pRemoteThread = VirtualAllocEx(hproc, NULL, 10, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

WriteProcessMemory(hproc,pRemoteThread,code,10,0);

hThread=CreateRemoteThread(hproc,0,0,pRemoteThread,pRemoteThread,0,&dwThreadId);

WaitForSingleObject(hThread, INFINITE);

GetExitCodeThread(hThread, (PDWORD) &exitcode);

printf(">exitcode is 0x%08x\n",exitcode);

VirtualFreeEx(hproc, pRemoteThread, 0, MEM_RELEASE);
/*********************************************************************************/
    if (!CloseHandle(hproc)) {
        printf("[E]: Process (%d) cannot be closed !\n", pid);
        return 2;
    };
    printf("[I]: Process (%d) is closed. \n", pid);
    return 0;
}