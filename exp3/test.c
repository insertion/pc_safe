#include<windows.h>
//字符串的地址需要自己填写，函数的地址也需要自己填写
char *s ="i'm hacked!";

char code[] ={0x6a,0x00,
			        0xeb,0x0d,
			        0x5b,
			        0x53,
			        0x53,
			        0x6a,0x00,
			        0xb8,0x1e,0xfd,0x14,0x75,//od查看的MessageBoxA的物理地址
              0xff,0xd0,
			        0xc3,
			        0xe8,0xee,0xff,0xff,0xff,
			        "i hacked you!"};
              //0x7514fd1e MessageBoxAd的物理地址
/*			  
  00000004: 6A 00              push        0
  00000006: EB 0D              jmp         00000015
  00000008: 5B                 pop         ebx
  00000009: 53                 push        ebx
  0000000A: 53                 push        ebx
  0000000B: 6A 00              push        0
  0000000D: B8 BB AA BB AA     mov         eax,0AABBAABBh
  00000012: FF D0              call        eax
  00000014: C3                 ret
  00000015: E8 EE FF FF FF     call        00000008
                               .string     "SOS"
*/			  
int i=(int)MessageBoxA;	

 
void main()
{
  //code[10]= (i)     &255;
  //code[11]= (i>>8)  &255;
  //code[12]= (i>>16) &255;
  //code[13]= (i>>24) &255;
	int op;
	VirtualProtect(&code[0],27,PAGE_EXECUTE_READWRITE,&op);
	__asm
	{
		call offset code
	}
	//能不能是call code
  //call code:         00401281  |. FF15 10304100    CALL DWORD PTR DS:[413010]
  //call offset code:  00401281  |. E8 8A1D0100      CALL test.00413010




}