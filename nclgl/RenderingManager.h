#pragma once
#include "Renderer.h"
#include "RenderStage.h"

enum renderStages {SIMPLE_STAGE, MAX_STAGE};

class RenderingManager
{
public:
	RenderingManager(Renderer* renderer);
	~RenderingManager();

	virtual void UpdateScene(float msec);
	virtual void DrawScene();

	inline void SetActiveScene(Scene* activeScene) { 
		this->activeScene = activeScene; 
		renderer->LoadLists(activeScene->GetTransparentNodeList(), activeScene->GetNodeList());
	}


protected:
	Renderer*	 renderer;
	RenderStage* renderStages[MAX_STAGE];

	Scene* activeScene;

	
	
};

