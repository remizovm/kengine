#ifndef KENGINE_DIB_H_
#define KENGINE_DIB_H_

#include "common.h"
#include "window.h"

namespace kengine {

class Dib {
 public:
	Dib(const Window* window);
	int Init();
	void Clear();
	void Update();
	void PutPixel(unsigned int x, unsigned int y, unsigned char r, 
								unsigned char g, unsigned char b);
 private:
	HWND hwnd_;
	unsigned int width_;
	unsigned int height_;
	HBITMAP	canvas_bmp_;
	HDC	canvas_dc_;
	unsigned char* canvas_buffer_;
	HGDIOBJ	obj_;
	PAINTSTRUCT	ps_;
	HDC dc_;
};

}

#endif