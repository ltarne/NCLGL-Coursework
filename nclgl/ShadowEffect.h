#pragma once
#include "PostProcessingEffect.h"
class ShadowEffect :
	public PostProcessingEffect
{
public:
	ShadowEffect(int width, int height, Shader* sceneShader, Shader* processingShader);
	~ShadowEffect();



};

