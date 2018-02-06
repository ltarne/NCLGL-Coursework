#include "PresentStage.h"



PresentStage::PresentStage(Renderer* renderer) 
	: RenderStage(renderer)
{
	stageShader = new Shader(SHADERDIR"combineVert.vert", SHADERDIR"screenCombineFrag.frag");
	stageShader->LinkProgram();

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
}


PresentStage::~PresentStage()
{
}

void PresentStage::DrawStage(Scene * scene) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(stageShader->GetProgram());
	glUniformMatrix4fv(glGetUniformLocation(stageShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	glUniform1i(glGetUniformLocation(stageShader->GetProgram(), "screenTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, colourBuffers[0]);


	quad->Draw();

	glUseProgram(0);
}
