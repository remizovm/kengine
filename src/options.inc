; starts the option checking timer (see dlgproc.inc -> on_timer_event)
; arguments:
; - hwnddlg: handle of the main dialogbox
proc options_init, hwnddlg
	invoke	SetTimer, [hwnddlg], KENGINE_OPTIONS_TIMER_ID, 10, 0
	ret
endp

; checks if the option's key was pressed and applies the option if so
; uses eax register
; arguments:
; - lpOption: pointer to the KOPTION struct
proc option_check uses eax, lpOption
	mov	eax, [lpOption]
	mov	eax, [eax + KOPTION.Key]
	; check if the option's key was pressed
	invoke	GetAsyncKeyState, eax
	aaa
	jz	.KeyWasNotPressed
	stdcall option_apply, [lpOption]
.KeyWasNotPressed:
	ret
endp

; applies the option according to it's type field
; uses eax register
; arguments:
; - lpOption: pointer to the KOPTION struct
proc option_apply uses eax, lpOption
	; save lpOption address in the EAX
	mov	eax, [lpOption]
	; run the needed apply action that is bound to option's type
	.if	[eax + KOPTION.Type] = KENGINE_OPTION_TYPE_TOGGLE
		stdcall option_apply_simple, [lpOption]
	.endif
	ret
endp

; writes option's payload or original bytes to the option's address
; and sets IsApplied to 0 or 1 accordingly
; uses eax, ebx, ecx and edx registers
; arguments:
; - lpOption: pointer to the KOPTION struct
proc option_apply_simple uses eax ebx ecx edx, lpOption
	; save lpOption address in the EAX
	mov	eax, [lpOption]
	; ebx = Address
	mov	ebx, [eax + KOPTION.Address]
	; ecx = IsApplied
	mov	ecx, [eax + KOPTION.IsApplied]
	; check if option is already applied
	.if	ecx
		; if yes, then we need to write the original bytes
		lea	ecx, [eax + KOPTION.OriginalBytes]
		; and set IsApplied to 0
		push	eax
		lea	eax, [eax + KOPTION.IsApplied]
		mov	[eax], dword 0
		pop	eax
	.else
		; if no, then we need to write the payload
		lea	ecx, [eax + KOPTION.Payload]
		; and set IsApplied to 1
		push	eax
		lea	eax, [eax + KOPTION.IsApplied]
		mov	[eax], dword 1
		pop	eax
	.endif
	; writing the data
	stdcall mem_write, [pID], ebx, ecx, [eax + KOPTION.PayloadSize]
	ret
endp