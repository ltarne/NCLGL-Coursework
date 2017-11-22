#include "RenderStage.h"

Mesh* RenderStage::quad = NULL;

RenderStage::RenderStage(Renderer* renderer) {
	this->renderer = renderer;
	this->bufferFBO = renderer->GetBufferFBO();
	this->colourBuffers = renderer->GetColourBuffers();
	this->bufferDepthTex = renderer->GetBufferDepthTex();
	stageShader = NULL;
}


RenderStage::~RenderStage() {
	if (stageShader) {
		delete stageShader;
	}
	
}

void RenderStage::DrawStage(Scene * scene) {
	renderer->SetViewMatrix(scene->GetViewMatrix());
	renderer->SetProjMatrix(scene->GetProjMatrix());
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	renderer->RenderScene();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderStage::DrawCombineQuad() {
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);

	glUniformMatrix4fv(glGetUniformLocation(stageShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	glUniform1i(glGetUniformLocation(stageShader->GetProgram(), "screenTex"), 2);
	glUniform1i(glGetUniformLocation(stageShader->GetProgram(), "lightTex"), 3);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, colourBuffers[0]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, colourBuffers[1]);


	quad->Draw();

	glUseProgram(0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
