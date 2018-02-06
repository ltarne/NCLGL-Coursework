#include "RenderingManager.h"



RenderingManager::RenderingManager(Renderer* renderer) {
	this->renderer = renderer;

	RenderStage::GenerateQuad();

	renderStages[BASIC_STAGE] = new RenderStage(renderer);
	renderStages[SHADOW_STAGE] = new ShadowStage(renderer);
	renderStages[DEFERRED_LIGHT_STAGE] = new DefereredLightsStage(renderer);
	renderStages[BLOOM_STAGE] = new BloomStage(renderer);
	renderStages[COLOUR_CORRECTION_STAGE] = new ColourCorrectionStage(renderer);
	renderStages[PRESENT_STAGE] = new PresentStage(renderer);
	textStage = new TextStage(renderer);
	renderStages[TEXT_STAGE] = textStage;
	

}


RenderingManager::~RenderingManager() {
	RenderStage::DeleteQuad();
	delete renderStages[BASIC_STAGE];
	delete renderStages[SHADOW_STAGE];
	delete renderStages[DEFERRED_LIGHT_STAGE];
	delete renderStages[BLOOM_STAGE];
	delete renderStages[COLOUR_CORRECTION_STAGE];
	delete renderStages[PRESENT_STAGE];
	delete renderStages[TEXT_STAGE];
}

void RenderingManager::UpdateScene(float msec) {
	activeScene->Update(msec);
	renderer->SetViewMatrix(activeScene->GetViewMatrix());
}

void RenderingManager::DrawScene() {
	vector<RenderStages> stages = activeScene->GetRenderStages();
	//Stages specific to scene
	for (int i = 0; i < stages.size(); ++i) {
		renderStages[stages[i]]->DrawStage(activeScene);
	}
	//Post processing effects
	for (map<RenderStages, bool>::iterator x = postProcessingSelect.begin(); x != postProcessingSelect.end(); ++x) {
		if (x->second) {
			renderStages[x->first]->DrawStage(activeScene);
		}
	}

	//Final Stages that are always done
	renderStages[PRESENT_STAGE]->DrawStage(activeScene);
	renderStages[TEXT_STAGE]->DrawStage(activeScene);

	renderer->SwapBuffers();
}


