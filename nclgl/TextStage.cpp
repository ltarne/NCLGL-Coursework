#include "TextStage.h"



TextStage::TextStage(Renderer* renderer) 
	: RenderStage(renderer) {
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	fontShader = new Shader(SHADERDIR"textVert.vert", SHADERDIR"textFrag.frag");
	fontShader->LinkProgram();

	viewMatrix = Matrix4();
	projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float) renderer->GetWidth(), 0.0f, (float) renderer->GetHeight(), 0.0f);
}


TextStage::~TextStage() {
	delete basicFont;
	delete fontShader;
}

void TextStage::DrawStage(Scene* scene) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	int size = 20;
	Vector3 position = Vector3(0, 0, 0);


	glUseProgram(fontShader->GetProgram());
	modelMatrix = Matrix4::Translation(Vector3(position.x, renderer->GetHeight() - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
	glUniformMatrix4fv(glGetUniformLocation(fontShader->GetProgram(), "modelMatrix"), 1, false, (float*) &modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(fontShader->GetProgram(), "viewMatrix"), 1, false, (float*) &viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(fontShader->GetProgram(), "projMatrix"), 1, false, (float*) &projMatrix);

	glUniform1i(glGetUniformLocation(fontShader->GetProgram(), "font"), 0);

	renderer->DrawText(to_string(renderer->GetFPS()), basicFont);

	glUseProgram(0);

	glDisable(GL_BLEND);
}
