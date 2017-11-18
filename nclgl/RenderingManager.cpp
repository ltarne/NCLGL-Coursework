#include "RenderingManager.h"



RenderingManager::RenderingManager(Renderer* renderer) {
	this->renderer = renderer;

	RenderStage::GenerateQuad();

	renderStages[SHADOW_STAGE] = new DefereredLightsStage(renderer);
	renderStages[PRESENT_STAGE] = new PresentStage(renderer);
	renderStages[TEXT_STAGE] = new TextStage(renderer);

}


RenderingManager::~RenderingManager() {
	RenderStage::DeleteQuad();
	delete renderStages[SHADOW_STAGE];
	delete renderStages[TEXT_STAGE];
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


