proc mem_init, proc_name
        stdcall mem_find_pid, [proc_name]
        ret
endp

proc mem_destroy
        ret
endp

proc mem_write uses eax ebx, pid, address, payload, payload_size
locals
        process_handle   dd ?
        old_protection   dd 0
endl
        .if     [pid] = 0
                ret
        .endif
        .if     [address] = 0
                ret
        .endif
        .if     [payload_size] = 0
                ret
        .endif
        invoke  OpenProcess, PROCESS_VM_OPERATION + PROCESS_VM_WRITE, 0, [pid]
        .if     eax = 0
                ; TODO: Check error.
                ret
        .endif
        mov     [process_handle], eax
        lea     ebx, [old_protection]

        invoke  VirtualProtectEx, eax, [address], [payload_size], PAGE_EXECUTE_READWRITE, ebx
        .if     eax = 0
                invoke  CloseHandle, [process_handle]
                ret
        .endif
        invoke  WriteProcessMemory, [process_handle], [address], [payload], [payload_size], 0
        .if     eax = 0
                invoke  VirtualProtectEx, [process_handle], [address], [payload_size], [old_protection], 0
                invoke  CloseHandle, [process_handle]
                ret
                ; TODO: Check error.
        .endif
        invoke  VirtualProtectEx, [process_handle], [address], [payload_size], [old_protection], 0
        invoke  CloseHandle, [process_handle]
        ret
endp

proc mem_find_pid, proc_name
locals
        hSnapshot    dd             ?
        pe32         PROCESSENTRY32 ?
endl
        invoke  CreateToolhelp32Snapshot, TH32CS_SNAPPROCESS, 0
        .if     eax = INVALID_HANDLE_VALUE
                ret
        .endif
        mov     [hSnapshot], eax
        mov     [pe32.dwSize], sizeof.PROCESSENTRY32
        lea     ebx, [pe32]
        invoke  Process32First, [hSnapshot], ebx
        .if     eax = 1
@@:
                lea     eax, [pe32.szExeFile]
                stdcall strcmp, [proc_name], eax
                .if     eax = 0
                        mov     eax, [pe32.th32ProcessID]
                        mov     [pID], eax
                        jmp     @f
                .endif
                invoke  Process32Next, [hSnapshot], ebx
                .if     eax <> 0
                        jmp     @b
                .endif
        .endif
        mov     [pID], 0
@@:
        invoke  CloseHandle, [hSnapshot]
        ret
endp