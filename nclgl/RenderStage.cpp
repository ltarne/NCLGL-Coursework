#include "RenderStage.h"

Mesh* RenderStage::quad = NULL;

RenderStage::RenderStage(Renderer* renderer) {
	this->renderer = renderer;
	this->bufferFBO = renderer->GetBufferFBO();
	this->colourBuffers = renderer->GetColourBuffers();
}


RenderStage::~RenderStage() {
}

void RenderStage::DrawStage(Scene * scene) {
	renderer->SetViewMatrix(scene->GetViewMatrix());
	renderer->SetProjMatrix(scene->GetProjMatrix());
	renderer->RenderScene();
}
