#part1
程序被加载后被分为三个section，分别为：
* .text 00401000  5000byte
* .rdara 00406000 2000byte
* .data 00408000 20000byte

0x0040100F这个地址所对应的指令 push eax
ebp-4 所存放的32位数为：4

MessageBox的前五条指令为

```asm
mov edi，edi
push ebp
mov ebo,esp
push 0
push dword ptr ss:[ebp+14]
```
后三天指令为
```asm
call user32.MessageBoxEXa
pop ebp
retn 10(这里的10是16进制的10)
```



#part4
如果自己的过程没有使用_declspec(naked)，那么系统会自动给添加一些额外的代码，控制堆栈平衡
'push ebp,mov ebp,esp'

psuh ecx是把to的eip压入堆栈，下一步ret 弹出给eip寄存器
eax 保存临时变量，且不首先作为源操作数(有点类似temp的功能，所以不需要保存)