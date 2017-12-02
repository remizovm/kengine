; FILENAME: kengine.asm

include 'win32axp.inc'

include 'macros.inc'
include 'const.inc'
include 'structs.inc'

.code
; main entry point
start:
        stdcall main
        invoke  ExitProcess,eax

; creates the main window and starts the main window loop
proc main
; in:  nothing
; out: last window message status
locals
        ; window class structure
        wc      WNDCLASS
        ; window message
        msg     MSG
endl
        ; fill the wc structure
        invoke  GetModuleHandle, 0
        mov     [wc.hInstance], eax
        invoke  LoadCursor, 0, IDC_ARROW
        mov     [wc.hCursor], eax
        mov     [wc.style], 0
        mov     [wc.lpfnWndProc], window_proc
        mov     [wc.cbClsExtra], 0
        mov     [wc.cbWndExtra], 0
        mov     [wc.hIcon], 0
        mov     [wc.hbrBackground], COLOR_BTNFACE+1
        mov     [wc.lpszMenuName], 0
        mov     [wc.lpszClassName], class_name
        ; register the main window class
        invoke  RegisterClass, addr wc
        ; TODO: check error
        ; reate the main window.
        invoke  CreateWindowEx, 0, class_name, "kengine", \
                                WS_VISIBLE+WS_POPUP+WS_SYSMENU, 128, 128,\
                                [window_width], [window_height], NULL, NULL,\
                                [wc.hInstance], NULL
        ; TODO: check error
        ; main window message loop
        .while
                invoke  GetMessage, addr msg, 0, 0, 0
                .if     eax = 0
                        .break
                .endif
                invoke  TranslateMessage, addr msg
                invoke  DispatchMessage, addr msg
        .endw
        ; place the return value to eax
        mov     eax, [msg.message]
        ret
endp

include 'helpers.inc'
include 'wndproc.inc'
include 'mem.inc'
include 'font.inc'

.end start

.data
; process id
pID               dd          ?
; process name
pname             db          'your_process_name.exe',0
class_name        db          'kengine',0

; gui
canvas            BITMAPINFO
ps                PAINTSTRUCT
hDC               dd          ?
canvas_dc         dd          ?
canvas_bmp        dd          ?
canvas_buffer     dd          ?
bpp               dw          32
window_width      dd          256
window_height     dd          192

letters           LETTERS

version           db 'version 1.0.0'