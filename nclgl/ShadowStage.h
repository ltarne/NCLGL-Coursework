#pragma once
#include "RenderStage.h"
class ShadowStage :
	public RenderStage
{
public:
	ShadowStage(Renderer* renderer);
	~ShadowStage();

	virtual void DrawStage(Scene* scene);

protected:
	void CreateShadowTextures(Scene * scene);
	void PresentScene(Scene * scene);

	Shader* shadowShader;
};

