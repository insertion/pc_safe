cl /c hello.c
cl /c myprint.c
link /out:hello.exe  hello.obj myprint.obj
hello.exe