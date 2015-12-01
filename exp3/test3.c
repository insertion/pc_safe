#include<windows.h>

char code[]={ 0xeb,0x1d,
			        
              0xb8,0x00,0x00,0x00,0x00,//LoadLibraryA的地址
			        0xff,0xd0,
		0xeb,0x24,
			        
              0x50,
			        0xb8,0x00,0x00,0x00,0x00,//GetProcAddress的地址
					    0xff,0xd0,
              0x6a,0x00,
		0xeb,0x29,
					    
              0x5b,
					    0x53,
              0x53,
				     	0x6a,0x00,
					    0xff,0xd0,
					    0xc3,
					    0xe8,0xde,0xff,0xff,0xff,//LoadLibraryA的参数
              'u','s','e','r','3','2','.','d','l','l',0x00,
			        0xe8,0xd7,0xff,0xff,0xff,//GetprocAddress的参数
              'M','e','s','s','a','g','e','B','o','x','A',0x00,
			        0xe8,0xd2,0xff,0xff,0xff,//MessageBoxAc参数的
			        "successfully!"
              };
/*
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
*/
int b=(int)LoadLibraryA;
int c=(int)GetProcAddress;
 
void main()
{
  code[3]= (b)     &255;
  code[4]= (b>>8)  &255;
  code[5]= (b>>16) &255;
  code[6]= (b>>24) &255;
  
  code[13]= (c)     &255;
  code[14]= (c>>8)  &255;
  code[15]= (c>>16) &255;
  code[16]= (c>>24) &255;
  
  
	int op;
	VirtualProtect(&code[0],27,PAGE_EXECUTE_READWRITE,&op);
	__asm
	{
		call offset code
	}





}