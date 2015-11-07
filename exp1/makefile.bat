cl /c 1add.c
link /DYNAMICBASE:no /out:1add.exe 1add.obj

cl /c 2sum.c
link /DYNAMICBASE:no /out:2sum.exe 2sum.obj

cl /c 3main.c 3sum.c
link /DYNAMICBASE:no /out:3sum.exe 3main.obj 3sum.obj
 
