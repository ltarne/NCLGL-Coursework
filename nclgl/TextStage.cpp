#include "TextStage.h"



TextStage::TextStage(Renderer* renderer) 
	: RenderStage(renderer) {
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	stageShader = new Shader(SHADERDIR"textVert.vert", SHADERDIR"textFrag.frag");
	stageShader->LinkProgram();

	viewMatrix = Matrix4();
	projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float) renderer->GetWidth(), 0.0f, (float) renderer->GetHeight(), 0.0f);
}


TextStage::~TextStage() {
	delete basicFont;
}

void TextStage::DrawStage(Scene* scene) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int fpsSize = 25;
	int settingsSize = 20;

	string bloomText = "BLOOM:";
	bloomText.append(bloom ? "ON" : "OFF");

	string greenText = "GREEN SHIFT:";
	greenText.append(green ? "ON" : "OFF");
	


	WriteToScreen(to_string(renderer->GetFPS()), Vector3(0, 0, 1), fpsSize);
	WriteToScreen(paused ? "TIMER PAUSED" : "", Vector3(renderer->GetWidth()/2 - fpsSize * 6, 0, 1), fpsSize);
	WriteToScreen(bloomText , Vector3(0, fpsSize, 1), settingsSize);
	WriteToScreen(greenText, Vector3(0, fpsSize + settingsSize , 1), settingsSize);
	

	glDisable(GL_BLEND);
}

void TextStage::WriteToScreen(string message, Vector3 position, int size) {
	

	glUseProgram(stageShader->GetProgram());
	modelMatrix = Matrix4::Translation(Vector3(position.x, renderer->GetHeight() - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
	glUniformMatrix4fv(glGetUniformLocation(stageShader->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(stageShader->GetProgram(), "viewMatrix"), 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(stageShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	glUniform1i(glGetUniformLocation(stageShader->GetProgram(), "font"), 0);

	renderer->DrawText(message, basicFont);

	glUseProgram(0);
}


