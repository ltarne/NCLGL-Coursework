#pragma once
#include "RenderStage.h"
class TextStage :
	public RenderStage {
public:
	TextStage(Renderer* renderer);
	~TextStage();

	virtual void DrawStage(Scene* scene);

protected:
	Font* basicFont;
	Shader* fontShader;

	Matrix4 projMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
};

