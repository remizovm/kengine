include 'gui.inc'

proc dialog_proc uses edi esi ebx, hwnddlg, msg, wparam, lparam
        .if     [msg] = WM_LBUTTONDOWN
                invoke  SendMessage, [hwnddlg], WM_NCLBUTTONDOWN, 2, 0
                jmp     .processed
        .elseif [msg] = WM_TIMER
                stdcall gui_clear_screen
                stdcall gui_draw
                invoke  RedrawWindow, [hwnddlg], 0, 0, RDW_INVALIDATE
                jmp     .processed
        .elseif [msg] = WM_PAINT
                invoke  BeginPaint, [hwnddlg], ps
                invoke  BitBlt, eax, 0, 0, [window_width], [window_height], [canvas_dc], 0, 0, SRCCOPY
                invoke  EndPaint, [hwnddlg], ps
                jmp     .processed
        .elseif [msg] = WM_INITDIALOG
                stdcall gui_init, [hwnddlg]
                jmp     .processed
        .elseif [msg] = WM_CLOSE | [msg] = WM_RBUTTONUP
                invoke  EndDialog, [hwnddlg], 0
        .endif
        jmp     .finish
.processed:
        mov     eax, 1
.finish:
        ret
endp