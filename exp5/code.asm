Microsoft (R) COFF/PE Dumper Version 14.00.23026.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file code.obj

File Type: COFF OBJECT

_code:
  00000000: 60                 pushad
  00000001: E8 00 00 00 00     call        $_delta$3
$_delta$3:
  00000006: 5B                 pop         ebx
  00000007: 81 EB 00 00 00 00  sub         ebx,offset $_delta$3
  0000000D: B8 00 00 40 00     mov         eax,400000h
  00000012: 03 83 00 00 00 00  add         eax,dword ptr $_addr_orign_aoep$4[ebx]
  00000018: 89 44 24 1C        mov         dword ptr [esp+1Ch],eax
  0000001C: 61                 popad
  0000001D: 50                 push        eax
  0000001E: 33 C0              xor         eax,eax
  00000020: C3                 ret
$_addr_orign_aoep$4:
  00000021: 4B                 dec         ebx
  00000022: 15 00 00 CC CC     adc         eax,0CCCC0000h
  00000027: CC                 int         3
  00000028: CC                 int         3
  00000029: CC                 int         3
  0000002A: CC                 int         3
  0000002B: CC                 int         3
  0000002C: CC                 int         3
  0000002D: CC                 int         3
  0000002E: CC                 int         3
  0000002F: CC                 int         3
_code_end:
  00000030: BB

  Summary

          78 .debug$S
          2F .drectve
          31 .text$mn
