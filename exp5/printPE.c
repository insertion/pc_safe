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
static DWORD PEAlign(DWORD dwTarNum,DWORD dwAlignTo)
{	
    //对齐处理
    return (((dwTarNum+dwAlignTo - 1) / dwAlignTo) * dwAlignTo);
}
/******************************************************************/
WORD  wInjSecNo;
//DWORD dwSizeOfCode = 0x4;
BYTE pCode[] ={0xCC, 0xCC, 0xCC, 0xCC};
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
     rsize =PEAlign(sizeof(pCode),pOptHeader->FileAlignment) ;

    // 请填入代码
     vsize = sizeof(pCode);
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
    pSecHeader[wInjSecNo]->Characteristics      = 0x60000020;
    //设置新的section可执行,权限和.text一样
    pSecHeader[wInjSecNo]->Name[0] = '.';
    pSecHeader[wInjSecNo]->Name[1] = 'm';
    pSecHeader[wInjSecNo]->Name[2] = 'y';
    pSecHeader[wInjSecNo]->Name[3] = 'c';
    pSecHeader[wInjSecNo]->Name[4] = 'o';
    pSecHeader[wInjSecNo]->Name[5] = 'd';
    pSecHeader[wInjSecNo]->Name[6] = 'e';
    pSecHeader[wInjSecNo]->Name[7] = '\0';
    
    pSecData[wInjSecNo] = (PBYTE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, rsize);
    //分配的大小应该是对齐后的物理大小
    dwSecSize[wInjSecNo] = pSecHeader[wInjSecNo]->SizeOfRawData;
    //这个值和rsize实际上时相等的
    CopyMemory(pSecData[wInjSecNo], pCode, sizeof(pCode));
    wSecNum++;
    pFileHeader->NumberOfSections = wSecNum;
    //section数++
    /* 对所有的段做对齐处理
    for(i = 0;i< wSecNum; i++) {
        pSecHeader[i]->VirtualAddress =
            PEAlign(pSecHeader[i]->VirtualAddress, pOptHeader->SectionAlignment);
        pSecHeader[i]->Misc.VirtualSize =
            PEAlign(pSecHeader[i]->Misc.VirtualSize, pOptHeader->SectionAlignment);
        
        pSecHeader[i]->PointerToRawData =
            PEAlign(pSecHeader[i]->PointerToRawData, pOptHeader->FileAlignment);
        
        pSecHeader[i]->SizeOfRawData = 
            PEAlign(pSecHeader[i]->SizeOfRawData, pOptHeader->FileAlignment);
    }
    */
    //内存镜像大小，rva+内存对齐后的大小
    //SizeOfImage这个值的大小，该值是所有节和头按照SectionAlignment 对齐后的大小
    pOptHeader->SizeOfImage = pSecHeader[wSecNum-1]->VirtualAddress + PEAlign(pSecHeader[wSecNum-1]->Misc.VirtualSize,pOptHeader->SectionAlignment);
    //磁盘文件大小应该是最后一个字节即Num-1的偏移+磁盘对齐后的大小
    dwFileSize = pSecHeader[wSecNum-1]->PointerToRawData + pSecHeader[wSecNum - 1]->SizeOfRawData;
    //边界清0是必须的
    pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
    pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
    return TRUE;
    //大部分PE文件都不使用文件头中的CheckSum域的校验和值
    //不过有些PE文件，如关键的系统服务程序文件以及驱动程序文件则该值必须正确，否则系统加载器将拒绝加载
}
/******************************************************************/


static DWORD RVA2Ptr(DWORD dwBaseAddress, DWORD dwRva)
{
    if ((dwBaseAddress != 0) && dwRva)
        return (dwBaseAddress + dwRva);
    else
        return dwRva;
}
//----------------------------------------------------------------
static PIMAGE_SECTION_HEADER RVA2Section(DWORD dwRVA)
{
    int i;
    for(i = 0; i < wSecNum; i++) {
        if ( (dwRVA >= pSecHeader[i]->VirtualAddress)
            && (dwRVA <= (pSecHeader[i]->VirtualAddress 
                + pSecHeader[i]->SizeOfRawData)) ) {
            return ((PIMAGE_SECTION_HEADER)pSecHeader[i]);
        }
    }
    return NULL;
}

//----------------------------------------------------------------
static PIMAGE_SECTION_HEADER Offset2Section(DWORD dwOffset)
{
    int i;
    for(i = 0; i < wSecNum; i++) {
        if( (dwOffset>=pSecHeader[i]->PointerToRawData) 
            && (dwOffset<(pSecHeader[i]->PointerToRawData + pSecHeader[i]->SizeOfRawData)))
        {
            return ((PIMAGE_SECTION_HEADER)pSecHeader[i]);
        }
    }
    return NULL;
}

//================================================================
static DWORD RVA2Offset(DWORD dwRVA)
{
    PIMAGE_SECTION_HEADER pSec;
    pSec = RVA2Section(dwRVA);//ImageRvaToSection(pimage_nt_headers,Base,dwRVA);
    if(pSec == NULL) {
        return 0;
    }
    return (dwRVA + (pSec->PointerToRawData) - (pSec->VirtualAddress));
}
//----------------------------------------------------------------
static DWORD Offset2RVA(DWORD dwOffset)
{
    PIMAGE_SECTION_HEADER pSec;
    pSec = Offset2Section(dwOffset);
    if(pSec == NULL) {
        return (0);
    }
    return(dwOffset + (pSec->VirtualAddress) - (pSec->PointerToRawData));
}

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
    LPCSTR lpszInjFileName = "hello_inj0.exe";
    
    hHeap = GetProcessHeap();
    if (! CopyPEFileToMem(lpszFileName)) {
        return 1;
    }
    AddNewSection();
    OutputPEInMem();
     if (! CopyMen2File(lpszInjFileName)) {
        return 1;
    }
    return 0;
}