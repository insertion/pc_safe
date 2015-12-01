Microsoft (R) COFF/PE Dumper Version 14.00.23026.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file exercise3.obj

File Type: COFF OBJECT

_main:
  00000004: EB 1D              jmp         00000023
  00000006: B8 BB AA BB AA     mov         eax,0AABBAABBh
  0000000B: FF D0              call        eax
  0000000D: EB 1A              jmp         00000029
  0000000F: 50                 push        eax
  00000010: B8 BB AA BB AA     mov         eax,0AABBAABBh
  00000015: FF D0              call        eax
  00000017: 6A 00              push        0
  00000019: EB 14              jmp         0000002F
  0000001B: 5B                 pop         ebx
  0000001C: 53                 push        ebx
  0000001D: 53                 push        ebx
  0000001E: 6A 00              push        0
  00000020: FF D0              call        eax
  00000022: C3                 ret
  00000023: E8 DE FF FF FF     call        00000006
  00000028: 90                 nop
  00000029: E8 DF FF FF FF     call        0000000D
  0000002E: 90                 nop
  0000002F: E8 E7 FF FF FF     call        0000001B
  00000034: 90                 nop
  Summary

          7C .debug$S
          5D .drectve
          38 .text$mn
