cl /c msg.c
link /DLL /DYNAMICBASE msg.obj

cl /c hellow.c
link  hellow.obj msg.lib