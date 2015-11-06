cl /c add.c
link /DYNAMICBASE:no /out:add.exe add.obj

cl /c sum.c
link /DYNAMICBASE:no /out:sum.exe sum.obj

