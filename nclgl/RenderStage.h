#pragma once
#include "Scene.h"

class RenderStage
{
public:
	RenderStage(OGLRenderer* renderer);
	~RenderStage();

	virtual void DrawStage(Scene* scene);

protected:
	OGLRenderer* renderer;

};

