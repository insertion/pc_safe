#include <windows.h>
#include <stdio.h>
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")
 HWND        hMainWnd, hWnd;
 DWORD       dwSizeOfCode;
 PBYTE       pCode, pOrignCode;
void shellcode();
void _addr_Setwindw();
void _addr_prehandle();
void _addr_handle();
void code_end();
__declspec(naked) void shellcode()
{
    __asm
    {
        //新建的远程线程和原线程共用一个栈吗？
        push ebx
        call s
    s:
        pop ebx
        sub ebx,offset s
        
        lea eax,[ebx+_newproc]
        push eax           //新函数的地址
        push GWL_WNDPROC   //表示替换窗体处理函数
        mov eax,[ebx+_addr_handle]
        push eax           //窗体句柄
        mov eax,[ebx+_addr_Setwindw]
        call eax
        mov [ebx+_addr_prehandle],eax
        pop ebx
        ret
        
        _newproc:
                 push ebx
                 call s2
             s2:
                 pop ebx
                 sub ebx,offset s2
               
                 mov eax,[ebx+_addr_prehandle]
                 pop ebx
                 
                 jmp eax
                 // ret     0x10
    }
}
__declspec(naked) void _addr_Setwindw()
{
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
    
}
__declspec(naked) void _addr_prehandle()
{
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
    
}
__declspec(naked) void _addr_handle()
{
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
    
}
__declspec(naked) void code_end()
    {
        __asm _emit 0xcc
    }

DWORD make_code()
{
    int off; 
    __asm {
        mov edx, offset shellcode
        mov dword ptr [pOrignCode], edx
        mov eax, offset code_end
        sub eax, edx
        mov dword ptr [dwSizeOfCode], eax
    }
    pCode = VirtualAlloc(NULL, dwSizeOfCode, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (pCode== NULL) {
        printf("[E]: VirtualAlloc failed\n");
        return 0;
    }
    for (off = 0; off<dwSizeOfCode; off++) {
        *(pCode+off) = *(pOrignCode+off);
    }
    *(PDWORD)((PBYTE)_addr_handle-(DWORD)pOrignCode+(DWORD)pCode) = (DWORD)hWnd;
    *(PDWORD)((PBYTE)_addr_Setwindw-(DWORD)pOrignCode+(DWORD)pCode) = (DWORD)SetWindowLongA;
    
    return dwSizeOfCode;
}
 
 int main()
 {
   //并通过窗体句柄得到notepad.exe的pid：
    DWORD       pid;
    HANDLE hproc = 0;
    DWORD dwThreadId;
    PVOID pRemoteThread ;
    HANDLE  hThread	= 0;
  // "WndMsg"
    hMainWnd = FindWindow("notepad", NULL);
    hWnd = hMainWnd;//GetWindow(hMainWnd, GW_CHILD);
    GetWindowThreadProcessId(hWnd, &pid);
 
    make_code();
 
    hproc = OpenProcess(PROCESS_CREATE_THREAD 
        | PROCESS_QUERY_INFORMATION
        | PROCESS_VM_OPERATION 
        | PROCESS_VM_WRITE 
        | PROCESS_VM_READ,
        FALSE, pid);

      pRemoteThread = 
          VirtualAllocEx(hproc, NULL, dwSizeOfCode, 
          MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

      WriteProcessMemory(hproc,pRemoteThread,pCode,dwSizeOfCode,0);
      
      hThread=CreateRemoteThread(hproc,0,0,pRemoteThread,pRemoteThread,0,&dwThreadId);
      
       if (!CloseHandle(hproc)) 
          {
             printf("[E]: Process (%d) cannot be closed !\n", pid);
             return 2;
          };
       printf("[I]: Process (%d) is closed. \n", pid);
       return 0;
 }
/*       
DWORD __stdcall NewProc(HWND hwnd,       // handle to window         
                        UINT uMsg,       // message identifier       
                        WPARAM wParam,   // first message parameter 
                        LPARAM lParam )  // second message parameter
                        
                        
DWORD __stdcall SetWindowLongA(HWND hWnd, int nIndex, LONG dwNewLong);  
//GWL_WNDPROC -4 为窗口过程设定一个新的地址。
*/                