cl /c test.c
link /dynamicbase:no test.obj user32.lib 
cl /c exercise3.c
cl /c test3.c
link /dynamicbase:no  test3.obj user32.lib
cl /c test4.c
link /dynamicbase:no test4.obj user32.lib
