Microsoft (R) COFF/PE Dumper Version 14.00.23026.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file shellcode.obj

File Type: COFF OBJECT

_CompStr:
  00000000: 55                 push        ebp
  00000001: 8B EC              mov         ebp,esp
  00000003: 83 EC 08           sub         esp,8
  00000006: 8B 45 08           mov         eax,dword ptr [ebp+8]
  00000009: 89 45 FC           mov         dword ptr [ebp-4],eax
  0000000C: 8B 4D 0C           mov         ecx,dword ptr [ebp+0Ch]
  0000000F: 89 4D F8           mov         dword ptr [ebp-8],ecx
  00000012: EB 12              jmp         00000026
  00000014: 8B 55 FC           mov         edx,dword ptr [ebp-4]
  00000017: 83 C2 01           add         edx,1
  0000001A: 89 55 FC           mov         dword ptr [ebp-4],edx
  0000001D: 8B 45 F8           mov         eax,dword ptr [ebp-8]
  00000020: 83 C0 01           add         eax,1
  00000023: 89 45 F8           mov         dword ptr [ebp-8],eax
  00000026: 8B 4D FC           mov         ecx,dword ptr [ebp-4]
  00000029: 0F BE 11           movsx       edx,byte ptr [ecx]
  0000002C: 85 D2              test        edx,edx
  0000002E: 74 20              je          00000050
  00000030: 8B 45 F8           mov         eax,dword ptr [ebp-8]
  00000033: 0F BE 08           movsx       ecx,byte ptr [eax]
  00000036: 85 C9              test        ecx,ecx
  00000038: 74 16              je          00000050
  0000003A: 8B 55 FC           mov         edx,dword ptr [ebp-4]
  0000003D: 0F BE 02           movsx       eax,byte ptr [edx]
  00000040: 8B 4D F8           mov         ecx,dword ptr [ebp-8]
  00000043: 0F BE 11           movsx       edx,byte ptr [ecx]
  00000046: 3B C2              cmp         eax,edx
  00000048: 74 04              je          0000004E
  0000004A: 33 C0              xor         eax,eax
  0000004C: EB 07              jmp         00000055
  0000004E: EB C4              jmp         00000014
  00000050: B8 01 00 00 00     mov         eax,1
  00000055: 8B E5              mov         esp,ebp
  00000057: 5D                 pop         ebp
  00000058: C3                 ret
  00000059: CC                 int         3
  0000005A: CC                 int         3
  0000005B: CC                 int         3
  0000005C: CC                 int         3
  0000005D: CC                 int         3
  0000005E: CC                 int         3
  0000005F: CC                 int         3
_GetIAFromImpDesc:
  00000060: 55                 push        ebp
  00000061: 8B EC              mov         ebp,esp
  00000063: 83 EC 0C           sub         esp,0Ch
  00000066: 8B 45 10           mov         eax,dword ptr [ebp+10h]
  00000069: 83 78 0C 00        cmp         dword ptr [eax+0Ch],0
  0000006D: 75 04              jne         00000073
  0000006F: 33 C0              xor         eax,eax
  00000071: EB 6C              jmp         000000DF
  00000073: 8B 4D 10           mov         ecx,dword ptr [ebp+10h]
  00000076: 8B 55 08           mov         edx,dword ptr [ebp+8]
  00000079: 03 11              add         edx,dword ptr [ecx]
  0000007B: 89 55 FC           mov         dword ptr [ebp-4],edx
  0000007E: 8B 45 10           mov         eax,dword ptr [ebp+10h]
  00000081: 8B 4D 08           mov         ecx,dword ptr [ebp+8]
  00000084: 03 48 10           add         ecx,dword ptr [eax+10h]
  00000087: 89 4D F8           mov         dword ptr [ebp-8],ecx
  0000008A: EB 12              jmp         0000009E
  0000008C: 8B 55 FC           mov         edx,dword ptr [ebp-4]
  0000008F: 83 C2 04           add         edx,4
  00000092: 89 55 FC           mov         dword ptr [ebp-4],edx
  00000095: 8B 45 F8           mov         eax,dword ptr [ebp-8]
  00000098: 83 C0 04           add         eax,4
  0000009B: 89 45 F8           mov         dword ptr [ebp-8],eax
  0000009E: 8B 4D FC           mov         ecx,dword ptr [ebp-4]
  000000A1: 83 39 00           cmp         dword ptr [ecx],0
  000000A4: 74 37              je          000000DD
  000000A6: 8B 55 FC           mov         edx,dword ptr [ebp-4]
  000000A9: 8B 02              mov         eax,dword ptr [edx]
  000000AB: 25 00 00 00 80     and         eax,80000000h
  000000B0: 74 02              je          000000B4
  000000B2: EB D8              jmp         0000008C
  000000B4: 8B 4D FC           mov         ecx,dword ptr [ebp-4]
  000000B7: 8B 55 08           mov         edx,dword ptr [ebp+8]
  000000BA: 03 11              add         edx,dword ptr [ecx]
  000000BC: 89 55 F4           mov         dword ptr [ebp-0Ch],edx
  000000BF: 8B 45 F4           mov         eax,dword ptr [ebp-0Ch]
  000000C2: 83 C0 02           add         eax,2
  000000C5: 50                 push        eax
  000000C6: 8B 4D 0C           mov         ecx,dword ptr [ebp+0Ch]
  000000C9: 51                 push        ecx
  000000CA: E8 00 00 00 00     call        _CompStr
  000000CF: 83 C4 08           add         esp,8
  000000D2: 85 C0              test        eax,eax
  000000D4: 74 05              je          000000DB
  000000D6: 8B 45 F8           mov         eax,dword ptr [ebp-8]
  000000D9: EB 04              jmp         000000DF
  000000DB: EB AF              jmp         0000008C
  000000DD: 33 C0              xor         eax,eax
  000000DF: 8B E5              mov         esp,ebp
  000000E1: 5D                 pop         ebp
  000000E2: C3                 ret
  000000E3: CC                 int         3
  000000E4: CC                 int         3
  000000E5: CC                 int         3
  000000E6: CC                 int         3
  000000E7: CC                 int         3
  000000E8: CC                 int         3
  000000E9: CC                 int         3
  000000EA: CC                 int         3
  000000EB: CC                 int         3
  000000EC: CC                 int         3
  000000ED: CC                 int         3
  000000EE: CC                 int         3
  000000EF: CC                 int         3
_GetIAFromImportTable:
  000000F0: 55                 push        ebp
  000000F1: 8B EC              mov         ebp,esp
  000000F3: 83 EC 14           sub         esp,14h
  000000F6: 8B 45 08           mov         eax,dword ptr [ebp+8]
  000000F9: 89 45 FC           mov         dword ptr [ebp-4],eax
  000000FC: 8B 4D FC           mov         ecx,dword ptr [ebp-4]
  000000FF: 8B 55 08           mov         edx,dword ptr [ebp+8]
  00000102: 03 51 3C           add         edx,dword ptr [ecx+3Ch]
  00000105: 89 55 F8           mov         dword ptr [ebp-8],edx
  00000108: 8B 45 F8           mov         eax,dword ptr [ebp-8]
  0000010B: 83 C0 18           add         eax,18h
  0000010E: 89 45 F4           mov         dword ptr [ebp-0Ch],eax
  00000111: B9 08 00 00 00     mov         ecx,8
  00000116: C1 E1 00           shl         ecx,0
  00000119: 8B 55 F4           mov         edx,dword ptr [ebp-0Ch]
  0000011C: 8B 44 0A 60        mov         eax,dword ptr [edx+ecx+60h]
  00000120: 89 45 F0           mov         dword ptr [ebp-10h],eax
  00000123: 8B 4D 08           mov         ecx,dword ptr [ebp+8]
  00000126: 03 4D F0           add         ecx,dword ptr [ebp-10h]
  00000129: 89 4D EC           mov         dword ptr [ebp-14h],ecx
  0000012C: 8B 55 EC           mov         edx,dword ptr [ebp-14h]
  0000012F: 52                 push        edx
  00000130: 8B 45 0C           mov         eax,dword ptr [ebp+0Ch]
  00000133: 50                 push        eax
  00000134: 8B 4D 08           mov         ecx,dword ptr [ebp+8]
  00000137: 51                 push        ecx
  00000138: E8 00 00 00 00     call        _GetIAFromImpDesc
  0000013D: 83 C4 0C           add         esp,0Ch
  00000140: 8B E5              mov         esp,ebp
  00000142: 5D                 pop         ebp
  00000143: C3                 ret

  Summary

          7C .debug$S
          5D .drectve
         144 .text$mn
