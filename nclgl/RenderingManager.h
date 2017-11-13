#pragma once
#include "Renderer.h"
#include "RenderStage.h"
#include "Scene.h"

#define RENDER_STAGES 1

class RenderingManager
{
public:
	RenderingManager(OGLRenderer* renderer);
	~RenderingManager();

	virtual void UpdateScene();
	virtual void DrawScene();

	void SetActiveScene(Scene* activeScene) { this->activeScene = activeScene; }


protected:
	OGLRenderer*	renderer;
	RenderStage*	renderStages[RENDER_STAGES];

	Scene* activeScene;
	
};

