cl /c test.c
link /dynamicbase:no test.obj user32.lib
cl /c hello.c
link /dynamicbase:no hello.obj
cl /c inj.c
link /dynamicbase:no inj.obj