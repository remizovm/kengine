#ifndef KENGINE_RENDERER_H_
#define KENGINE_RENDERER_H_

namespace kengine {

class Renderer {
 public:
	virtual void Clear() = 0;
	virtual void Update() = 0;
};

}

#endif