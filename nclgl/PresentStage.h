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
	Shader* shader;

	Matrix4 projMatrix;
};

