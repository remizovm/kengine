proc strcmp uses ebx esi edi,lpStrA,lpStrB
        mov     esi, [lpStrA]
        mov     edi, [lpStrB]
@@:
        mov     al, [esi]
        mov     bl, [edi]
        .if     al <> bl
                jmp     .not_equal
        .endif
        .if     al = 0
                jmp .done
        .endif
        inc     esi
        inc     edi
        jmp     @b
.not_equal:
        xor     eax, eax
        dec     eax
        ret
.done:
        xor     eax, eax
        ret
endp