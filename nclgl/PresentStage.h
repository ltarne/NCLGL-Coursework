#pragma once
#include "RenderStage.h"
class PresentStage :
	public RenderStage
{
public:
	PresentStage(Renderer* renderer);
	~PresentStage();

	virtual void DrawStage(Scene* scene);

protected:

	Matrix4 projMatrix;
};

