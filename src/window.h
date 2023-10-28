#ifndef KENGINE_WINDOW_H_
#define KENGINE_WINDOW_H_

#include "common.h"

namespace kengine {

class Window {
 public:
	Window(const unsigned int width, const unsigned int height);
	~Window();
	int Init();
	void Update();
	unsigned int width_;
	unsigned int height_;
	HWND window_handle_;
	bool should_close_;
 private:
	static LRESULT CALLBACK WndProcWrapper(HWND hWnd, UINT msg, WPARAM wParam, 
																				 LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HINSTANCE instance_;
	ATOM wnd_class_;
	const TCHAR* kClassName = TEXT("kengine");
	const TCHAR* kWindowName = TEXT("kengine");
};

}

#endif