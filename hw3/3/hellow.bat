cl /c msg.c
link /DLL /DYNAMICBASE msg.obj

cl /c hellow.c
link /DYNAMICBASE:no /BASE:0x02000000 hellow.obj msg.lib