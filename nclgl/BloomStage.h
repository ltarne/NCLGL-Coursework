#pragma once
#include "RenderStage.h"

#define POST_PASSES 10

class BloomStage :
	public RenderStage
{
public:
	BloomStage(Renderer* renderer);
	~BloomStage();

	virtual void DrawStage(Scene* scene);

	void IsolateBrightAreas();
	void BlurBrightAreas();
	void Combine();

protected:
	Shader* seperationShader;
	Shader* blurShader;
	Shader* bloomCombineShader;

	GLuint blurFBO;
	GLuint blurColourBuffer;

};

