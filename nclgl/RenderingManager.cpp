#include "RenderingManager.h"



RenderingManager::RenderingManager(Renderer* renderer) {
	this->renderer = renderer;

	RenderStage::GenerateQuad();

	renderStages[BASIC_STAGE] = new RenderStage(renderer);
	renderStages[SHADOW_STAGE] = new ShadowStage(renderer);
	renderStages[DEFERRED_LIGHT_STAGE] = new DefereredLightsStage(renderer);
	renderStages[BLOOM_STAGE] = new BloomStage(renderer);
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
	renderer->SetViewMatrix(activeScene->GetViewMatrix());
}

void RenderingManager::DrawScene() {
	vector<RenderStages> stages = activeScene->GetRenderStages();
	for (int i = 0; i < stages.size(); ++i) {
		renderStages[stages[i]]->DrawStage(activeScene);
	}

	renderer->SwapBuffers();
}


