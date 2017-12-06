; FILENAME: kengine.asm

include 'win32axp.inc'

include 'macros.inc'
include 'const.inc'
include 'structs.inc'

.code
; main entry point
start:
        invoke  DialogBoxParam, <invoke  GetModuleHandle, 0>, 37, 0,\
                                dialog_proc, 0
        .if     eax = -1
                ; TODO: handle error
        .endif
        invoke  ExitProcess, eax

include 'helpers.inc'
include 'dlgproc.inc'
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

section '.rsrc' resource data readable

  directory RT_DIALOG,dialogs

  resource dialogs,\
           37,LANG_ENGLISH+SUBLANG_DEFAULT,main

  dialog main,'kengine',0,0,170,118,WS_POPUP
  enddialog
