cl /c msg.c
link /dll msg.obj
cl /c hello.c
link /dynamicbase:no hello.obj msg.lib 