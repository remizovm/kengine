include 'gui.inc'

proc window_proc uses ebx esi edi, hwnd,wmsg,wparam,lparam
        .if [wmsg] = WM_CREATE
                stdcall gui_init, [hwnd]
                ;stdcall mem_init, pname
                xor     eax, eax
                ret
        .elseif [wmsg] = WM_PAINT
                invoke  BeginPaint, [hwnd], ps
                invoke  BitBlt, eax, 0, 0, [window_width], [window_height], [canvas_dc], 0, 0, SRCCOPY
                invoke  EndPaint, [hwnd], ps
                xor     eax, eax
                ret
        .elseif [wmsg] = WM_LBUTTONDOWN
                invoke  SendMessage, [hwnd], WM_NCLBUTTONDOWN, 2, 0
                xor     eax, eax
                ret
        .elseif [wmsg] = WM_TIMER
                mov     edi, [canvas_buffer]
                mov     ecx, [window_width]
                imul    ecx, [window_height]
                xor     eax, eax
                rep     stosd
                mov     edi, [canvas_buffer]
                stdcall gui_draw
                invoke  RedrawWindow, [hwnd], 0, 0, RDW_INVALIDATE
                xor     eax, eax
                ret
        .elseif [wmsg] = WM_CLOSE | [wmsg] = WM_RBUTTONUP
                ;stdcall mem_destroy
                invoke  DestroyWindow,[hwnd]
                xor     eax, eax
                ret
        .elseif [wmsg] = WM_DESTROY
                invoke  PostQuitMessage, 0
                xor     eax, eax
                ret
        .endif
        invoke  DefWindowProc,[hwnd],[wmsg],[wparam],[lparam]
        ret
endp