#include "RenderingManager.h"



RenderingManager::RenderingManager(Renderer* renderer) {
	this->renderer = renderer;

	renderStages[SIMPLE_STAGE] = new RenderStage(renderer);

}


RenderingManager::~RenderingManager() {
	delete renderStages[SIMPLE_STAGE];
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


