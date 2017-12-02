include 'win32axp.inc'

include 'macros.inc'
include 'const.inc'
include 'structs.inc'

.code
start:
        invoke  GetModuleHandle, 0
        mov     [wc.hInstance], eax
        invoke  LoadCursor, 0, IDC_ARROW
        mov     [wc.hCursor], eax
        invoke  RegisterClass, wc
        invoke  CreateWindowEx, 0, class_name, window_title, \
                                WS_VISIBLE+WS_POPUP+WS_SYSMENU, 128, 128,\
                                [window_width], [window_height], NULL, NULL,\
                                [wc.hInstance], NULL
        .while
                invoke  GetMessage, msg, 0, 0, 0
                .if     eax = 0
                        .break
                .endif
                invoke  TranslateMessage,msg
                invoke  DispatchMessage,msg
        .endw
        invoke  ExitProcess,eax

include 'helpers.inc'
include 'wndproc.inc'
include 'mem.inc'
include 'font.inc'

.end start

.data
pID               dd          ?
pname             db          'your_process_name.exe',0
class_name        db          'kengine',0
window_title      db          'kengine',0
msg               MSG
wc                WNDCLASS    0,window_proc,0,0,0,0,0,COLOR_BTNFACE+1,0,\
                              class_name
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