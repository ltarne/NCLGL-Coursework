#include "DefereredLightsStage.h"



DefereredLightsStage::DefereredLightsStage(Renderer* renderer) 
	: RenderStage(renderer) {
	combineShader = new Shader(SHADERDIR"combineVert.vert", SHADERDIR"combineFrag.frag");
	combineShader->LinkProgram();

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	
	glGenFramebuffers(1, &geometryFBO);
	glGenFramebuffers(1, &pointLightFBO);


	GenerateScreenTexture(bufferDepthTex, true);
	GenerateScreenTexture(bufferColourTex);
	GenerateScreenTexture(bufferNormalTex);
	GenerateScreenTexture(bufferEmissiveTex);
	GenerateScreenTexture(bufferSpecularTex);

}


DefereredLightsStage::~DefereredLightsStage() {
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteTextures(1, &bufferColourTex);
	glDeleteTextures(1, &bufferNormalTex);
	glDeleteTextures(1, &bufferEmissiveTex);
	glDeleteTextures(1, &bufferSpecularTex);

	glDeleteFramebuffers(1, &pointLightFBO);
}

void DefereredLightsStage::AttachFBOData() {

	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;

	glBindFramebuffer(GL_FRAMEBUFFER, geometryFBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);

	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "FBO FAIL!";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferEmissiveTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferSpecularTex, 0);

	glDrawBuffers(2, buffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "FBO FAIL!";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

void DefereredLightsStage::DettachFBOData() {

}

void DefereredLightsStage::DrawStage(Scene * scene) {

	AttachFBOData();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	renderer->SetViewMatrix(scene->GetViewMatrix());
	renderer->SetProjMatrix(scene->GetProjMatrix());

	FillBuffers();
	DrawPointLights(scene);
	CombineBuffers(scene);

}

void DefereredLightsStage::FillBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, geometryFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	renderer->RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DefereredLightsStage::DrawPointLights(Scene* scene) {
	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glEnable(GL_BLEND);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE);

	glUseProgram((*(scene->GetLightList()->begin()))->GetShader()->GetProgram());
	vector<Texture*>* temp = (*(scene->GetLightList()->begin()))->GetTextures();

	((*temp)[0])->SetTexture(bufferDepthTex);
	((*temp)[1])->SetTexture(bufferNormalTex);

	renderer->DrawLights();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DefereredLightsStage::CombineBuffers(Scene* scene) {
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);

	glUseProgram(combineShader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(combineShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	glUniform1i(glGetUniformLocation(combineShader->GetProgram(), "diffuseTex"),2);
	glUniform1i(glGetUniformLocation(combineShader->GetProgram(), "emissiveTex"), 3);
	glUniform1i(glGetUniformLocation(combineShader->GetProgram(), "specularTex"), 4);
	glUniform1i(glGetUniformLocation(combineShader->GetProgram(), "depthTex"), 5);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bufferEmissiveTex);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bufferSpecularTex);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);

	
	quad->Draw();
	
	glUseProgram(0);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
