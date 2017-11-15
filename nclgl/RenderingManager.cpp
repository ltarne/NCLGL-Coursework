#include "RenderingManager.h"



RenderingManager::RenderingManager(Renderer* renderer) {
	this->renderer = renderer;

	renderStages[SHADOW_STAGE] = new ShadowStage(renderer);
	renderStages[TEXT_STAGE] = new TextStage(renderer);

}


RenderingManager::~RenderingManager() {
	delete renderStages[SHADOW_STAGE];
}

void RenderingManager::UpdateScene(float msec) {
	activeScene->Update(msec);
}

void RenderingManager::DrawScene() {
	for (int i = 0; i < MAX_STAGE; ++i) {
		renderStages[i]->DrawStage(activeScene);
	}

	renderer->SwapBuffers();
}


