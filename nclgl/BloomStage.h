#pragma once
#include "RenderStage.h"
class BloomStage :
	public RenderStage
{
public:
	BloomStage(Renderer* renderer);
	~BloomStage();
};

