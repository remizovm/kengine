proc mem_init, hwnd, proc_name
	invoke	SetTimer, [hwnd], KENGINE_MEM_TIMER_ID, 100, 0
	ret
endp

proc mem_destroy
	ret
endp

proc mem_update uses eax, pid, proc_name
	; get a new process id into eax
	stdcall mem_find_pid, [proc_name]
	.if	eax <= 0
		; pid was not found, so our current one is invalid
		.if	[pID]
			mov	[pID], 0
		.endif
		ret
	.endif
	; if it has not changed - return
	.if	[pID] = eax
		ret
	.endif
	; save new pid
	mov	[pID], eax
	ret
endp

proc mem_write uses eax ebx, pid, address, payload, payload_size
locals
	process_handle	 dd ?
	old_protection	 dd 0
endl
	.if	[pid] = 0
		ret
	.endif
	.if	[address] = 0
		ret
	.endif
	.if	[payload_size] = 0
		ret
	.endif
	invoke	OpenProcess, PROCESS_VM_OPERATION + PROCESS_VM_WRITE, 0, [pid]
	.if	eax = 0
		; TODO: Check error.
		ret
	.endif
	mov	[process_handle], eax
	invoke	VirtualProtectEx, eax, [address], [payload_size], PAGE_EXECUTE_READWRITE, addr old_protection
	.if	eax = 0
		invoke	CloseHandle, [process_handle]
		ret
	.endif
	invoke	WriteProcessMemory, [process_handle], [address], [payload], [payload_size], 0
	.if	eax = 0
		invoke	VirtualProtectEx, [process_handle], [address], [payload_size], [old_protection], 0
		invoke	CloseHandle, [process_handle]
		ret
		; TODO: Check error.
	.endif
	invoke	VirtualProtectEx, [process_handle], [address], [payload_size], [old_protection], 0
	invoke	CloseHandle, [process_handle]
	ret
endp

proc mem_find_pid, proc_name
locals
	hSnapshot    dd 	    ?
	pe32	     PROCESSENTRY32 ?
endl
	invoke	CreateToolhelp32Snapshot, TH32CS_SNAPPROCESS, 0
	.if	eax = INVALID_HANDLE_VALUE
		; TODO: handle error
		jmp	.not_found
	.endif
	mov	[hSnapshot], eax
	mov	[pe32.dwSize], sizeof.PROCESSENTRY32
	invoke	Process32First, [hSnapshot], addr pe32
	; TODO: handle error
	.if	eax = 1
@@:
		stdcall strcmp, [proc_name], addr pe32.szExeFile
		.if	eax = 0
			jmp	.found
		.endif
		invoke	Process32Next, [hSnapshot], addr pe32
		; TODO: handle error
		.if	eax <> 0
			jmp	@b
		.endif
	.endif
.not_found:
	; nothing was found, returning zero
	xor	eax, eax
	jmp	.exit
.found:
	mov	eax, [pe32.th32ProcessID]
.exit:
	.if	[hSnapshot]
		; saving eax register because CloseHandle will rewrite it
		push	eax
		invoke	CloseHandle, [hSnapshot]
		; TODO: handle error
		pop	eax
	.endif
	ret
endp