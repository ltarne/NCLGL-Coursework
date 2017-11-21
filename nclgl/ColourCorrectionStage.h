#pragma once
#include "RenderStage.h"
class ColourCorrectionStage :
	public RenderStage
{
public:
	ColourCorrectionStage(Renderer* renderer);
	~ColourCorrectionStage();

	virtual void DrawStage(Scene* scene);

protected:
	GLuint textureLookup;
};

