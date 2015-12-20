__declspec(naked) void code()
{
    __asm {
        pushad                        //PUSHAD指令压入32位寄存器，其入栈顺序是:EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI 
        call    _delta
_delta:
        pop     ebx                         // save registers context from stack
        sub     ebx, offset _delta

        mov     eax, 0x00400000
        add     eax,dword ptr [ebx + _addr_orign_aoep]
        mov     dword ptr [esp + 0x1C], eax     // save (dwBaseAddress + orign) to eax-location-of-context
        popad                                   // load registers context from stack
        push    eax
        xor     eax, eax
        retn                                    // >> jump to the Original AOEP
_addr_orign_aoep:
        _emit   0x4b
        _emit   0x15
        _emit   0x00
        _emit   0x00
    }
}
__declspec(naked) void code_end()
{
    __asm _emit 0xBB
}