#pragma once
#include "RenderStage.h"
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
	Shader* shader;
	Matrix4 projMatrix;

};

