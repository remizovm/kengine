#include "app.h"
#include "window.h"
#include "dib.h"

namespace kengine {

App::App() {}

int App::Run() {
	Window window(400, 300);
	window.Init();
	Dib dib(&window);
	dib.Init();
	while (!window.should_close_) {
		window.Update();
		dib.Clear();
		dib.PutPixel(10, 10, 255, 255, 255);
		dib.PutPixel(11, 11, 255, 255, 255);
		dib.PutPixel(11, 10, 255, 255, 255);
		dib.PutPixel(10, 11, 255, 255, 255);
		dib.Update();
	}
	return 0;
}

bool App::IsKeyPressed(const int key) {
	if (GetAsyncKeyState(key) & 0x01) {
		return true;
	}
	return false;
}

} // namespace kengine
