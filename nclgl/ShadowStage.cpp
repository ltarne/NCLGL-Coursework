#include "ShadowStage.h"



ShadowStage::ShadowStage(Renderer* renderer) 
	: RenderStage(renderer){
	shadowShader = new Shader(SHADERDIR"shadowVert.vert", SHADERDIR"shadowFrag.frag");
	shadowShader->LinkProgram();

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);
}


ShadowStage::~ShadowStage() {
	delete shadowShader;
}

void ShadowStage::DrawStage(Scene * scene) {

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	renderer->SetViewMatrix(scene->GetViewMatrix());
	renderer->SetProjMatrix(scene->GetProjMatrix());
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	CreateShadowTextures(scene);
	PresentScene(scene);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ShadowStage::CreateShadowTextures(Scene * scene) {
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	renderer->SetOverrideShader(shadowShader);

	renderer->SetViewMatrix(Matrix4::BuildViewMatrix(scene->GetLight()->GetPosition(), Vector3(0, 0, 0)));
	renderer->SetTextureMatrix(biasMatrix*(scene->GetProjMatrix() * Matrix4::BuildViewMatrix(scene->GetLight()->GetPosition(), Vector3(0, 0, 0))));

	renderer->RenderScene();

	renderer->SetOverrideShader(nullptr);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, renderer->GetWidth(), renderer->GetHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ShadowStage::PresentScene(Scene * scene) {
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	renderer->SetViewMatrix(scene->GetCamera()->BuildViewMatrix());

	renderer->RenderScene();

}
