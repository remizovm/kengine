#include "dib.h"

namespace kengine {

Dib::Dib(const Window* window) {
	hwnd_ = window->window_handle_;
	width_ = window->width_;
	height_ = window->height_;
	canvas_bmp_ = NULL;
	canvas_dc_ = NULL;
	canvas_buffer_ = NULL;
	obj_ = NULL;
	ps_ = { 0 };
	dc_ = NULL;
}

int Dib::Init() {
	BITMAPINFO canvas = { 0 };
	canvas.bmiHeader.biSize = sizeof(canvas.bmiHeader);
	canvas.bmiHeader.biWidth = width_;
	canvas.bmiHeader.biHeight = -(signed int(height_));
	canvas.bmiHeader.biPlanes = 1;
	canvas.bmiHeader.biBitCount = 32;
	canvas_dc_ = CreateCompatibleDC(GetWindowDC(hwnd_));
	canvas_bmp_ = CreateDIBSection(canvas_dc_, &canvas, DIB_RGB_COLORS, (LPVOID*)&canvas_buffer_, 0, 0);
	if (!canvas_dc_ || !canvas_bmp_) {
		// TODO: handle the error
		return 1;
	}
	dc_ = GetDC(hwnd_);
	obj_ = SelectObject(canvas_dc_, canvas_bmp_);
	SetBkMode(canvas_dc_, TRANSPARENT);
	return 0;
}

void Dib::Clear() {
	memset((void*)canvas_buffer_, 0, width_ * height_ * 4);
}

void Dib::Update() {
	BitBlt(dc_, 0, 0, width_, height_, canvas_dc_, 0, 0, SRCCOPY);
	RedrawWindow(hwnd_, 0, 0, RDW_INVALIDATE);
}

void Dib::PutPixel(unsigned int x, unsigned int y, unsigned char r, 
									 unsigned char g, unsigned char b) {
	unsigned int iOffset = 0;
	if (x >= 0 && x < width_ && y >= 0 && y < height_) {
		iOffset = ((y * width_) + x);
		canvas_buffer_[(iOffset * 4)] = b;
		canvas_buffer_[(iOffset * 4) + 1] = g;
		canvas_buffer_[(iOffset * 4) + 2] = r;
	}
}

} // namespace kengine