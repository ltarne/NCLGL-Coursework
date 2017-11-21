#include "ColourCorrectionStage.h"



ColourCorrectionStage::ColourCorrectionStage(Renderer* renderer) 
	: RenderStage(renderer) {
	stageShader = new Shader(SHADERDIR"combineVert.vert", SHADERDIR"colourCorrectionFrag.frag");
	stageShader->LinkProgram();

	textureLookup = SOIL_load_OGL_texture(TEXTUREDIR"correctionTable.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
}


ColourCorrectionStage::~ColourCorrectionStage() {
}

void ColourCorrectionStage::DrawStage(Scene * scene) {

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glUseProgram(stageShader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(stageShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	glUniform1i(glGetUniformLocation(stageShader->GetProgram(), "screenTex"), 2);
	glUniform1i(glGetUniformLocation(stageShader->GetProgram(), "correctionTex"), 3);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, colourBuffers[0]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureLookup);

	glDisable(GL_DEPTH_TEST);

	quad->Draw();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);

}
