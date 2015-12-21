cl /c hello.c
link /dynamicbase:no /base:0x400000 hello.obj
cl /c printPE.c
link  printPE.obj
cl printPE4.c