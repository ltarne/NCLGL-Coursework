#include "BloomStage.h"



BloomStage::BloomStage(Renderer* renderer) : RenderStage(renderer) {
	shader = new Shader(SHADERDIR"combineVert.vert", SHADERDIR"bloomFrag.frag");
	shader->LinkProgram();

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
}


BloomStage::~BloomStage() {
	delete shader;
}

void BloomStage::DrawStage(Scene * scene) {
	IsolateBrightAreas();
	BlurBrightAreas();
	Combine();
}

void BloomStage::IsolateBrightAreas() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	glUniform1i(glGetUniformLocation(shader->GetProgram(), "screenTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, colourBuffers[0]);

	glDisable(GL_DEPTH_TEST);

	quad->Draw();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
}

void BloomStage::BlurBrightAreas() {
}

void BloomStage::Combine() {
}
