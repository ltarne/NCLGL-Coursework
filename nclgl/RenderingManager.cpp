#include "RenderingManager.h"



RenderingManager::RenderingManager(OGLRenderer* renderer) {
	this->renderer = renderer;
	

}


RenderingManager::~RenderingManager() {
}

void RenderingManager::DrawScene() {
	for (int i = 0; i < RENDER_STAGES; ++i) {
		renderStages[i]->DrawStage(activeScene);
	}
}
