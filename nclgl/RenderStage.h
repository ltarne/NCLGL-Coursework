#pragma once
#include "Scene.h"
#include "Renderer.h"

class RenderStage
{
public:
	RenderStage(Renderer* renderer);
	~RenderStage();

	virtual void DrawStage(Scene* scene);

protected:
	Renderer* renderer;

	FrameBufferInfo* FBInfo;
	GLuint bufferFBO;

};

