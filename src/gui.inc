proc gui_init, hwnd
        mov     [canvas.bmiHeader.biSize], sizeof.BITMAPINFOHEADER
        mov     eax, [window_width]
        mov     [canvas.bmiHeader.biWidth], eax
        push    ebx
        mov     ebx, [window_height]
        neg     ebx
        mov     [canvas.bmiHeader.biHeight], ebx
        mov     bx, [bpp]
        mov     [canvas.bmiHeader.biBitCount], bx
        pop     ebx
        mov     [canvas.bmiHeader.biPlanes], 1
        invoke  GetDC, [hwnd]
        mov     [hDC], eax
        invoke  CreateCompatibleDC, eax
        mov     [canvas_dc], eax
        invoke  CreateDIBSection, [hDC], canvas, 0, canvas_buffer, 0, 0
        mov     [canvas_bmp], eax
        invoke  SelectObject, [canvas_dc], eax
        invoke  ReleaseDC, [hDC], 0
        invoke  SetTimer, [hwnd], 41.66, 0, 0
        ret
endp

proc gui_draw
        stdcall gui_draw_rect, 1, 1, 126, 94, COLOR_WHITE
        stdcall draw_string, version, 10, 10, COLOR_WHITE
        ret
endp

proc DrawPoint uses eax ebx, x,y,color
        mov     eax, [x]
        mov     ebx, [y]
        .if     eax <> 0
                shl     eax, 1
        .endif
        .if     ebx <> 0
                shl     ebx, 1
        .endif
        stdcall gui_draw_pixel, eax, ebx, [color]
        inc     eax
        stdcall gui_draw_pixel, eax, ebx, [color]
        inc     ebx
        stdcall gui_draw_pixel, eax, ebx, [color]
        dec     eax
        stdcall gui_draw_pixel, eax, ebx, [color]
        ret
endp

proc gui_draw_line uses eax ebx ecx esi edi edx,x1,y1,x2,y2,color
locals
        error dd 0
        dxx   dd 0
        dyy   dd 0
        x     dd 0
        y     dd 0
endl
        mov     eax, [x1]
        mov     ebx, [y1]
        mov     esi, [x2]
        mov     edi, [y2]
        mov     [error], 0
        mov     ecx, eax
        sub     ecx, esi
        mov     edx, ecx
        neg     ecx
        cmovl   ecx, edx
        mov     [dxx], ecx
        mov     ecx, ebx
        sub     ecx, edi
        mov     edx, ecx
        neg     ecx
        cmovl   ecx, edx
        mov     [dyy], ecx
        .if     [dxx] >= ecx
                jmp     .h_line
        .endif
        jmp     .v_line
.h_line:
        .if     eax>esi
                jmp      .swap_h
        .endif
        jmp     .start_paint_h
.swap_h:
        xchg    eax, esi
        xchg    ebx, edi
.start_paint_h:
        mov     ecx, [dxx]
        mov     [y], ebx
        mov     [x], eax
        stdcall DrawPoint, [x], [y], [color]
.loop_paint_h:
        inc     [x]
        stdcall DrawPoint, [x], [y], [color]
        mov     edx, [dyy]
        add     edx, [error]
        mov     [error], edx
        shl     edx, 1
        .if     edx >= [dxx]
                jmp     .inc_y
        .endif
        jmp     .next_step_h
.inc_y:
        inc     [y]
        mov     edx, [error]
        sub     edx, [dxx]
        mov     [error], edx
.next_step_h:
        loop    .loop_paint_h
        jmp     .exit_line
.v_line:
        .if     ebx > edi
                jmp      .swap_v
        .endif
        jmp     .start_paint_v
.swap_v:
        xchg    eax, esi
        xchg    ebx, edi
.start_paint_v:
        mov     ecx, [dyy]
        mov     [y], ebx
        mov     [x], eax
        stdcall DrawPoint, [x], [y], [color]
.loop_paint_v:
        inc     [y]
        stdcall DrawPoint, [x], [y], [color]
        mov     edx, [dxx]
        add     edx, [error]
        mov     [error], edx
        shl     edx, 1
        .if     edx >= [dyy]
                jmp     .inc_x
        .endif
        jmp     .next_step_v
.inc_x:
        inc     [x]
        mov     edx, [error]
        sub     edx, [dyy]
        mov     [error], edx
.next_step_v:
        loop    .loop_paint_v
.exit_line:
        ret
endp

proc gui_draw_rect x0,y0,x1,y1,color
        stdcall gui_draw_line, [x0], [y0], [x1], [y0], [color]
        stdcall gui_draw_line, [x1], [y0], [x1], [y1], [color]
        stdcall gui_draw_line, [x1], [y1], [x0], [y1], [color]
        stdcall gui_draw_line, [x0], [y1], [x0], [y0], [color]
        ret
endp

proc gui_draw_pixel uses eax ebx ecx edi, x,y,color
;ptr = (y * scanline) + (x * bpp / 8)
        xor     dx, dx
        mov     ax, [bpp]
        mov     bx, 8
        div     bx
        xchg    bx, ax
        mov     eax, [window_width]
        mul     bx
        mul     [y]
        mov     ebx, eax
        mov     eax, 4
        mul     [x]
        add     eax, ebx
        mov     edi, [canvas_buffer]
        add     edi, eax
        mov     ecx, [color]
        mov     [edi], ecx
        ret
endp