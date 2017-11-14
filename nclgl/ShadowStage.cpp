#include "ShadowStage.h"



ShadowStage::ShadowStage(Renderer* renderer) 
	: RenderStage(renderer){
	shadowShader = new Shader(SHADERDIR"shadowVert.vert", SHADERDIR"shadowFrag.frag");
	shadowShader->LinkProgram();
}


ShadowStage::~ShadowStage() {
	delete shadowShader;
}

void ShadowStage::DrawStage(Scene * scene) {
	renderer->SetViewMatrix(scene->GetViewMatrix());
	renderer->SetProjMatrix(scene->GetProjMatrix());
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	CreateShadowTextures(scene);
	PresentScene(scene);
}

void ShadowStage::CreateShadowTextures(Scene * scene) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBInfo->bufferFBO);
	

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
	glBindTexture(GL_TEXTURE_2D, FBInfo->shadowTex);

	renderer->SetViewMatrix(scene->GetCamera()->BuildViewMatrix());

	renderer->RenderScene();

}
