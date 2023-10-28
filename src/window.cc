#include "window.h"
#include "dib.h"

namespace kengine {

Window::Window(const unsigned int width, const unsigned int height) {
  instance_ = NULL;
  wnd_class_ = NULL;
  width_ = width;
  height_ = height;
  window_handle_ = NULL;
  should_close_ = false;
}

Window::~Window() {
  if (NULL != wnd_class_) {
    UnregisterClass(kClassName, instance_);
  }
}

int Window::Init() {
  instance_ = GetModuleHandle(NULL);
  if (NULL == instance_) {
    return 1;
  }

  WNDCLASSEX wc = { 0 };
  wc.cbSize = sizeof(wc);
  wc.lpszClassName = kClassName;
  wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hIcon = LoadIcon(instance_, IDI_APPLICATION);
  wc.hCursor = LoadCursor(instance_, IDC_ARROW);
  wc.hInstance = instance_;
  wc.lpfnWndProc = WndProcWrapper;
  wnd_class_ = RegisterClassEx(&wc);
  if (NULL == wnd_class_) {
    return 1;
  }
  window_handle_ = CreateWindowEx(WS_EX_TOOLWINDOW, kClassName, kWindowName, 
                                  WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 
                                  width_, height_, NULL, NULL, instance_, 
                                  this);
  if (NULL == window_handle_) {
    return 1;
  }
  ShowWindow(window_handle_, SW_NORMAL);
  UpdateWindow(window_handle_);
  return 0;
}

void Window::Update() {
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

LRESULT CALLBACK Window::WndProcWrapper(HWND hWnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam) {
  Window* pThis;
  if (msg == WM_NCCREATE) {
    LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
    pThis = static_cast<Window*>(lpcs->lpCreateParams);
    SetWindowLongPtr(hWnd, GWLP_USERDATA,
      reinterpret_cast<LONG_PTR>(pThis));
  }
  else {
    pThis = reinterpret_cast<Window*>(
      GetWindowLongPtr(hWnd, GWLP_USERDATA));
  }
  if (pThis) {
    return pThis->WndProc(hWnd, msg, wParam, lParam);
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
  case WM_CLOSE:
  case WM_RBUTTONUP:
    should_close_ = true;
    PostQuitMessage(0);
    break;
  case WM_LBUTTONDOWN:
    SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    break;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

} // namespace kengine