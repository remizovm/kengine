proc draw_letter uses ecx edx eax,letter,x,y,color
local rows:DWORD,x_offset:DWORD,y_offset:DWORD
    mov     edx,[letter]
    xor     ecx,ecx
    mov     [rows],0
    mov     [x_offset],0
    mov     [y_offset],0
.loop:
    mov     bl,[edx+ecx]
    cmp     bl,1
    jne     @f
    pushad
    mov     eax,[x_offset]
    add     eax,[x]
    mov     ebx,[y_offset]
    add     ebx,[y]
    stdcall DrawPoint,eax,ebx,[color]
    popad
@@:
    inc     [x_offset]
    cmp     [x_offset],5
    jne     @f
    inc     [y_offset]
    xor     eax,eax
    mov     [x_offset],eax
@@:
    inc     ecx
    cmp     ecx,0x19
    jne     .loop
    ret
endp

proc letter2hex uses ecx ebx edi,letter
    mov eax,letters
    mov ecx,38
@@:
    mov ebx,[eax+25]
    .if ebx=[letter]
        lea eax,[eax]
        ret
    .endif
    add eax,29
    loop @b
.finished:
    ret
endp

proc draw_string uses eax ebx,str,x,y,color
locals
.ccount dd 0
.lcount dd 0
endl
    xor eax,eax
.loop:
    mov     ebx,[str]
    push    eax
    movzx   eax,byte [ebx+eax]
    cmp     eax,0
    je      .exit
    stdcall letter2hex,eax
    cmp     eax,0
    je      @f
    stdcall draw_letter,eax,[x],[y],[color]
    cmp     [.ccount],31
    jne     .nonewline
    add     [y],6
    push    eax
    mov     eax,[.ccount]
    imul    eax,6
    sub     [x],eax
    pop     eax
    mov     [.ccount],0
    inc     [.lcount]
    jmp     @f
.nonewline:
    inc     [.ccount]
    add     [x],6
@@:
    pop     eax
    inc     eax
    jmp     .loop
.exit:
    ret
endp