#pragma once
#include "RenderStage.h"

//#define LIGHTNUM 8


class DefereredLightsStage :
	public RenderStage
{
public:
	DefereredLightsStage(Renderer* renderer);
	~DefereredLightsStage();

	virtual void DrawStage(Scene* scene);

protected:
	void FillBuffers();
	void DrawPointLights();
	void CombineBuffers();

	void GenerateScreenTexture(GLuint &into, bool depth = false);
};

