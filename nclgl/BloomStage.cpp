#include "BloomStage.h"



BloomStage::BloomStage(Renderer* renderer) : RenderStage(renderer) {
	seperationShader = new Shader(SHADERDIR"combineVert.vert", SHADERDIR"bloomFrag.frag");
	seperationShader->LinkProgram();

	blurShader = new Shader(SHADERDIR"combineVert.vert", SHADERDIR"blurFrag.frag");
	blurShader->LinkProgram();

	bloomCombineShader = new Shader(SHADERDIR"combineVert.vert", SHADERDIR"bloomCombineFrag.frag");
	bloomCombineShader->LinkProgram();

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);

	glGenFramebuffers(1, &blurFBO);

	glGenTextures(1, &blurColourBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);

	glBindTexture(GL_TEXTURE_2D, blurColourBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, renderer->GetWidth(), renderer->GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


BloomStage::~BloomStage() {
	delete seperationShader;
	delete bloomCombineShader;
	delete blurShader;
}

void BloomStage::DrawStage(Scene * scene) {
	IsolateBrightAreas();
	BlurBrightAreas();
	Combine();
}

void BloomStage::IsolateBrightAreas() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glUseProgram(seperationShader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(seperationShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	glUniform1i(glGetUniformLocation(seperationShader->GetProgram(), "screenTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, colourBuffers[0]);

	glDisable(GL_DEPTH_TEST);

	quad->Draw();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
}

void BloomStage::BlurBrightAreas() {
	glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
	glUseProgram(blurShader->GetProgram());
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColourBuffer, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//UpdateShaderMatrices(blurShader);
	glUniformMatrix4fv(glGetUniformLocation(blurShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	glUniform1i(glGetUniformLocation(blurShader->GetProgram(), "tex"), 2);
	glDisable(GL_DEPTH_TEST);

	glUniform2f(glGetUniformLocation(blurShader->GetProgram(), "pixelSize"), 1.0f / renderer->GetWidth(), 1.0f / renderer->GetHeight());

	for (int i = 0; i < POST_PASSES; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColourBuffer, 0);

		glUniform1i(glGetUniformLocation(blurShader->GetProgram(), "isVertical"), 0);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, colourBuffers[1]);
		quad->Draw();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourBuffers[1], 0);

		glUniform1i(glGetUniformLocation(blurShader->GetProgram(), "isVertical"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, blurColourBuffer);
		quad->Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
	//std::swap(colourBuffers[1], blurColourBuffer);
}

void BloomStage::Combine() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glUseProgram(bloomCombineShader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(bloomCombineShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	glUniform1i(glGetUniformLocation(bloomCombineShader->GetProgram(), "screenTex"), 2);
	glUniform1i(glGetUniformLocation(bloomCombineShader->GetProgram(), "lightTex"), 3);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, colourBuffers[0]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, colourBuffers[1]);

	glDisable(GL_DEPTH_TEST);

	quad->Draw();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
}
