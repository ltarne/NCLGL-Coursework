#pragma once
#include "Renderer.h"
#include "ShadowStage.h"
#include "DefereredLightsStage.h"
#include "TextStage.h"

enum renderStages {SHADOW_STAGE, TEXT_STAGE, MAX_STAGE};

class RenderingManager
{
public:
	RenderingManager(Renderer* renderer);
	~RenderingManager();

	virtual void UpdateScene(float msec);
	virtual void DrawScene();

	inline void SetActiveScene(Scene* activeScene) { 
		this->activeScene = activeScene; 
		renderer->SetActiveScene(this->activeScene);
	}


protected:
	Renderer*	 renderer;
	RenderStage* renderStages[MAX_STAGE];

	Scene* activeScene;

	
	
};

