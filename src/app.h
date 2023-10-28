#ifndef KENGINE_APP_H_
#define KENGINE_APP_H_

#include "common.h"

namespace kengine {

class App {
 public:
	App();
	int Run();
	bool IsKeyPressed(const int key);
};

} // namespace kengine

#endif