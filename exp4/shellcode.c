#include <windows.h>
BOOL CompStr(LPSTR s1, LPSTR s2)
{
    PCHAR p, q;
    for (p = s1, q = s2; (*p != 0) && (*q != 0); p++, q++) {
        if (*p != *q) return FALSE;
    }
    return TRUE;
}

DWORD GetIAFromImpDesc(DWORD dwBase, LPCSTR lpszName,PIMAGE_IMPORT_DESCRIPTOR pImpDesc) 
{
    PIMAGE_THUNK_DATA pthunk, pthunk2;
    PIMAGE_IMPORT_BY_NAME pOrdinalName;
    if (pImpDesc->Name == 0) return 0;
    //如果导入表的第一元素的名字为0，则没有dll
    //第一个dll为user32.dll
    pthunk = (PIMAGE_THUNK_DATA) (dwBase + pImpDesc->OriginalFirstThunk);
    pthunk2 = (PIMAGE_THUNK_DATA) (dwBase + pImpDesc->FirstThunk);
    
    for (; pthunk->u1.Function != 0; pthunk++, pthunk2++) {
        if (pthunk->u1.Ordinal & 0x80000000) continue;
        pOrdinalName = (PIMAGE_IMPORT_BY_NAME) (dwBase + pthunk->u1.AddressOfData);
        if (CompStr((LPSTR)lpszName, (LPSTR)&pOrdinalName->Name)) 
            return (DWORD)pthunk2;
    }
    return 0;
}

DWORD GetIAFromImportTable(DWORD dwBase, LPCSTR lpszFuncName)
{
 /*1*/   PIMAGE_DOS_HEADER pDosHeader =(PIMAGE_DOS_HEADER)dwBase;
 /*2*/   PIMAGE_NT_HEADERS pNtHeaders =(PIMAGE_NT_HEADERS)((int)dwBase + pDosHeader->e_lfanew);
 /*3*/   PIMAGE_OPTIONAL_HEADER32 pOptHeader = &(pNtHeaders->OptionalHeader);
 /*4*/   DWORD dwRVAImpTbl = pOptHeader->DataDirectory[1].VirtualAddress;
 /*5*/   PIMAGE_IMPORT_DESCRIPTOR pImpTbl =( PIMAGE_IMPORT_DESCRIPTOR) ((DWORD)dwBase + dwRVAImpTbl);
 return GetIAFromImpDesc(dwBase,lpszFuncName,pImpTbl);
 }
 