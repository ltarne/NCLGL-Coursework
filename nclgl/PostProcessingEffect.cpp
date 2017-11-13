#include "PostProcessingEffect.h"

Mesh* PostProcessingEffect::quad = NULL;

PostProcessingEffect::PostProcessingEffect(int width, int height, Shader* sceneShader, Shader* processingShader) {
	this->sceneShader = sceneShader;
	this->processShader = processingShader;
	this->width = width;
	this->height = height;
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();



}


PostProcessingEffect::~PostProcessingEffect() {

}

void PostProcessingEffect::Draw() {
	if (FBInfo == nullptr) {
		cout << "No FBInfo!!\n";
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, FBInfo->processFBO);
	glUseProgram(processShader->GetProgram());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBInfo->bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	UpdateShaderMatrices(processShader);
	glDisable(GL_DEPTH_TEST);

	glUniform2f(glGetUniformLocation(processShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

	for (int i = 0; i < POST_PASSES; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBInfo->bufferColourTex[1], 0);

		glUniform1i(glGetUniformLocation(processShader->GetProgram(), "isVertical"), 0);

		quad->SetTexture(FBInfo->bufferColourTex[0]);
		quad->Draw();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBInfo->bufferColourTex[0], 0);

		glUniform1i(glGetUniformLocation(processShader->GetProgram(), "isVertical"), 1);

		quad->SetTexture(FBInfo->bufferColourTex[1]);
		quad->Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
	std::swap(FBInfo->bufferColourTex[0],FBInfo->bufferColourTex[1]);
}

void PostProcessingEffect::DrawOnce() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBInfo->processFBO);
	glUseProgram(processShader->GetProgram());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBInfo->bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	UpdateShaderMatrices(processShader);
	glDisable(GL_DEPTH_TEST);

	glUniform2f(glGetUniformLocation(processShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

	quad->SetTexture(FBInfo->bufferColourTex[0]);
	quad->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
	std::swap(FBInfo->bufferColourTex[0], FBInfo->bufferColourTex[1]);
}

void PostProcessingEffect::Present() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(sceneShader->GetProgram());
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	UpdateShaderMatrices(sceneShader);
	quad->SetTexture(FBInfo->bufferColourTex[0]);
	quad->Draw();
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	
}

void PostProcessingEffect::UpdateShaderMatrices(Shader * s) {
	if (s) {
		glUniform1i(glGetUniformLocation(s->GetProgram(), "tex"), 0);

		glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);
		glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "viewMatrix"), 1, false, (float*)&viewMatrix);
		glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);
	}
}


