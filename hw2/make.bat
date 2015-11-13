cl /c msg.c hellow.c 
link /dll msg.obj user32.lib
link /out:hellow1.exe hellow.obj msg.lib
@hellow1.exe