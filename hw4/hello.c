

/*
    直接拿地址和偏移相加减是错的，要先把地址强制转换为int型
*/

#include <windows.h>
#include <stdio.h>
__declspec(dllimport) int ShowMsg(char *s);

char *s = "Hello world!";

int main()
{ 
    
    ShowMsg(s);
    // FILL YOUR CODE HERE
    //如何找到importTable？
    //首先找到该exe在内存中的基地址和IDD的RVA
    printf("********************************************************************\n");
 HMODULE dwBaseAddress  = GetModuleHandle(0);
 printf("* the baseAddress is 0x%p\n",dwBaseAddress);
                                                   
 PIMAGE_DOS_HEADER pDosHeader =        (PIMAGE_DOS_HEADER)dwBaseAddress;//基地址也就是dosheader的地址
 printf("* the Dos->e_lfanew is 0x%x\n",pDosHeader->e_lfanew);
 PIMAGE_NT_HEADERS pNtHeaders =        (PIMAGE_NT_HEADERS)((int)dwBaseAddress + pDosHeader->e_lfanew);
 //这里要把dwBaseAddress强制转换成int型，因为地址+1实际上是加4个字节
 printf("* the pNtHeaders is 0x%p\n",pNtHeaders);
                                                     /*
                                                    typedef struct _IMAGE_NT_HEADERS 
                                                    {
                                                    DWORD Signature;
                                                    IMAGE_FILE_HEADER FileHeader;
                                                    IMAGE_OPTIONAL_HEADER32 OptionalHeader;
                                                    } IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;
                                                    */
                                                    
 PIMAGE_OPTIONAL_HEADER32 pOptHeader = &(pNtHeaders->OptionalHeader);
 printf("* the pOptHeader is 0x%p\n",(void *)pOptHeader);
                                                    /*
                                                    typedef struct _IMAGE_OPTIONAL_HEADER 
                                                    {
                                                    ...
                                                        DWORD   ImageBase;
                                                    ...
                                                        IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
                                                    } IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;
                                                    */
 DWORD dwRVAImpTbl = pOptHeader->DataDirectory[1].VirtualAddress;
 printf("* the dwRVAImpTbl is %#x\n",dwRVAImpTbl);
 DWORD dwSizeOfImpTbl = pOptHeader->DataDirectory[1].Size;
 printf("* the dwsizeofimptable is %d\n",dwSizeOfImpTbl);
                                                    /*
                                                    typedef struct _IMAGE_DATA_DIRECTORY 
                                                    {
                                                    DWORD   VirtualAddress;
                                                    DWORD   Size;
                                                    } IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;
                                                    
                                                    */

 //基地址加上rva就是导入表在内存中的地址
 DWORD pImpTbl = (DWORD)dwBaseAddress + dwRVAImpTbl;
 printf("* the pImpTbl is %#x\n",pImpTbl);   
               
 PIMAGE_IMPORT_DESCRIPTOR p = (PIMAGE_IMPORT_DESCRIPTOR)pImpTbl;
                    //导入表也是一个结构体，有20个字节，每个导入表代表一个dll，最后一个以全0结束
 
  
                                                    /*
                                                    typedef struct _IMAGE_IMPORT_DESCRIPTOR 
                                                    {  
                                                        union {  
                                                            DWORD   Characteristics;  
                                                            DWORD   OriginalFirstThunk;  
                                                            } DUMMYUNIONNAME;  
                                                            DWORD   TimeDateStamp;  
                                                            DWORD   ForwarderChain;  
                                                            DWORD   Name;  //dll的名字的指针
                                                            DWORD   FirstThunk;  
                                                    } IMAGE_IMPORT_DESCRIPTOR;
  */
  //printf("the firstDLL is %s\n",p->Name+(DWORD)dwBaseAddress);  
 
  
 PIMAGE_IMPORT_DESCRIPTOR pImpDesc[5];
  int i;
 for (i=0; ;i++,p++) 
 {
        pImpDesc[i] = p;
        if (pImpDesc[i]->Name == 0) break; //如果检测到dll的名字为空，则跳出循环
          else 
        printf("* %s is linked in this program\n",(char *)(pImpDesc[i]->Name + (DWORD)dwBaseAddress));
 }
 printf("* there are %d dlls linked\n",i);  

                                                    /*
                                                    OriginalFirstThunk和FirstThunk具有相同的的结构
                                                    typedef struct _IMAGE_THUNK_DATA
                                                    {  
                                                            union {  
                                                                ULONGLONG ForwarderString;  // PBYTE   
                                                                ULONGLONG Function;         // PDWORD  
                                                                ULONGLONG Ordinal;  
                                                                ULONGLONG AddressOfData;    // PIMAGE_IMPORT_BY_NAME  
                                                            } u1;  
                                                    } IMAGE_THUNK_DATA;  
                                                    INT里面实际上存了IMAGE_IMPORT_BY_NAME的虚拟地址  
                                                    IAT里面存的是函数的虚拟地址
                                                    ******************************************************* 
                                                    typedef struct _IMAGE_IMPORT_BY_NAME 
                                                    {  
                                                            WORD    Hint;  
                                                            BYTE    Name[1];  
                                                    } IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;  
                                                    */
  DWORD * PINT= (DWORD *)(pImpDesc[0]->OriginalFirstThunk+(DWORD)dwBaseAddress);
  PIMAGE_IMPORT_BY_NAME fName ;
  //分别得到INT和IAT的地址，INT里面的rva指向函数名，IAT里为函数地址
  DWORD * PIAT = (DWORD *)(pImpDesc[0]->FirstThunk+(DWORD)dwBaseAddress);
  printf("********************************************************************\n");
 for(;;)
  {
     fName = (PIMAGE_IMPORT_BY_NAME)((*PINT)+(DWORD)dwBaseAddress);
     if( *PINT == 0) break;
     printf("%s : function is %s : address is %#x\n",(char *)(pImpDesc[0]->Name + (DWORD)dwBaseAddress),&(fName->Name),*PIAT);
      PINT++;
      PIAT++;   
 }   
 //pImpDesc[1]是kernel32.dll
 //pImpDesc[0]是msg.dll
 //鉴于msg.dll里只有一个函数，效果不明显，可以把pImpDesc[0]改为pImpDesc[1]来查看kernel32.dll里的函数
 //以下程序是查看kernel32.dll里的函数
   DWORD * kPINT= (DWORD *)(pImpDesc[1]->OriginalFirstThunk+(DWORD)dwBaseAddress);
   PIMAGE_IMPORT_BY_NAME kfName ;
   DWORD * kPIAT = (DWORD *)(pImpDesc[1]->FirstThunk+(DWORD)dwBaseAddress);
  printf("********************************************************************\n");
 for(;;)
  {
     kfName = (PIMAGE_IMPORT_BY_NAME)((*kPINT)+(DWORD)dwBaseAddress);
     if( *kPINT == 0) break;
     printf("%s : function is %s : address is %#x\n",(char *)(pImpDesc[1]->Name + (DWORD)dwBaseAddress),&(kfName->Name),*kPIAT);
      kPINT++;
      kPIAT++;
  }  
         
    return 0;
}