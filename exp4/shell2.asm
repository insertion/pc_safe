Microsoft (R) COFF/PE Dumper Version 14.00.23026.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file shellcode2.obj

File Type: COFF OBJECT

_main:
  00000000: 55                 push        ebp
  00000001: 8B EC              mov         ebp,esp
  00000003: 51                 push        ecx
  00000004: 8D 45 FC           lea         eax,[ebp-4]
  00000007: 50                 push        eax
  00000008: 6A 40              push        40h
  0000000A: 6A 04              push        4
  0000000C: 8B 0D 00 00 00 00  mov         ecx,dword ptr [_entryPoint]
  00000012: 51                 push        ecx
  00000013: FF 15 00 00 00 00  call        dword ptr [__imp__VirtualProtect@16]
  00000019: 33 C0              xor         eax,eax
  0000001B: 8B E5              mov         esp,ebp
  0000001D: 5D                 pop         ebp
  0000001E: C3                 ret

  Summary

          80 .debug$S
          5D .drectve
          1F .text$mn
