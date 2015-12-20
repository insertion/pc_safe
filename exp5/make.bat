cl /c hello.c
link /dynamicbase:no hello.obj
cl /c printPE.c
link /dynamicbase:no printPE.obj