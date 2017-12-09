include 'gui.inc'

proc on_timer_event hwnddlg, timer_id, timer_callback
	.if	[timer_id] = KENGINE_GUI_TIMER_ID
		stdcall gui_clear_screen
		stdcall gui_draw
		invoke	RedrawWindow, [hwnddlg], 0, 0, RDW_INVALIDATE
	.elseif [timer_id] = KENGINE_MEM_TIMER_ID
		stdcall mem_update, [pID], pname
	.elseif [timer_id] = KENGINE_OPTIONS_TIMER_ID
	.endif
	ret
endp

proc dialog_proc uses edi esi ebx, hwnddlg, msg, wparam, lparam
	.if	[msg] = WM_LBUTTONDOWN
		invoke	SendMessage, [hwnddlg], WM_NCLBUTTONDOWN, 2, 0
		jmp	.processed
	.elseif [msg] = WM_TIMER
		stdcall on_timer_event, [hwnddlg], [wparam], [lparam]
		jmp	.processed
	.elseif [msg] = WM_PAINT
		invoke	BeginPaint, [hwnddlg], ps
		invoke	BitBlt, eax, 0, 0, [window_width], [window_height], [canvas_dc], 0, 0, SRCCOPY
		invoke	EndPaint, [hwnddlg], ps
		jmp	.processed
	.elseif [msg] = WM_INITDIALOG
		stdcall gui_init, [hwnddlg]
		jmp	.processed
	.elseif [msg] = WM_CLOSE | [msg] = WM_RBUTTONUP
		invoke	KillTimer, [hwnddlg], KENGINE_MEM_TIMER_ID
		invoke	KillTimer, [hwnddlg], KENGINE_GUI_TIMER_ID
		invoke	EndDialog, [hwnddlg], 0
	.endif
	jmp	.finish
.processed:
	mov	eax, 1
.finish:
	ret
endp