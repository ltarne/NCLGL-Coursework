#include "RenderStage.h"



RenderStage::RenderStage(OGLRenderer* renderer) {
	this->renderer = renderer;
}


RenderStage::~RenderStage() {
}

void RenderStage::DrawStage(Scene * scene) {
	renderer->RenderScene();
}
