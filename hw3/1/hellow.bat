cl /c msg.c
link /DLL /DYNAMICBASE msg.obj

cl /c hellow.c
link /DYNAMICBASE:no /BASE:0x00400000 hellow.obj msg.lib