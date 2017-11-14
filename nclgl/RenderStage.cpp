#include "RenderStage.h"



RenderStage::RenderStage(Renderer* renderer) {
	this->renderer = renderer;
	this->FBInfo = renderer->GetFBInfo();
}


RenderStage::~RenderStage() {
}

void RenderStage::DrawStage(Scene * scene) {
	renderer->SetViewMatrix(scene->GetViewMatrix());
	renderer->SetProjMatrix(scene->GetProjMatrix());
	renderer->RenderScene();
}
