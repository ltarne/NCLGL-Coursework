#include "DefereredLightsStage.h"



DefereredLightsStage::DefereredLightsStage(Renderer* renderer) 
	: RenderStage(renderer) {
}


DefereredLightsStage::~DefereredLightsStage() {
}

void DefereredLightsStage::DrawStage(Scene * scene) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	FillBuffers();
	DrawPointLights();
	CombineBuffers();

}

void DefereredLightsStage::FillBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBInfo->bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	renderer->RenderScene();


}

void DefereredLightsStage::DrawPointLights() {
}

void DefereredLightsStage::CombineBuffers() {
}

void DefereredLightsStage::GenerateScreenTexture(GLuint & into, bool depth) {
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0,
		depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8,
		renderer->GetWidth(), renderer->GetHeight(), 0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA,
		GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}
