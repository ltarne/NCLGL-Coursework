#include "LightNode.h"



LightNode::LightNode(Shader * shader, Mesh * mesh, Vector4 colour, float radius, Vector3 pos) : SceneNode(shader, mesh, colour) {
	this->radius = radius;
	this->position = pos;
	transform = Matrix4::Translation(pos);
	scale = Matrix4::Scale(Vector3(radius, radius, radius));
}

LightNode::~LightNode() {
}

void LightNode::LoadUniforms() {
	Shader* activeShader = overrideShader == nullptr ? shader : overrideShader;

	Matrix4 modelMatrix = worldTransform * scale;
	glUniformMatrix4fv(glGetUniformLocation(activeShader->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);

	glUniform3fv(glGetUniformLocation(activeShader->GetProgram(), "lightPos"), 1, (float*)&position);
	glUniform4fv(glGetUniformLocation(activeShader->GetProgram(), "lightColour"), 1, (float*)&colour);
	glUniform1f(glGetUniformLocation(activeShader->GetProgram(), "lightRadius"), radius);

	for (int i = 0; i < textures.size() && i < TEXTURE_UNIT_MAX; ++i) {
		textures[i]->LoadTexture(activeShader->GetProgram(), i);
	}
}


void LightNode::UpdateTextures(GLuint depthTex, GLuint normalTex) {

}

void LightNode::Draw(const OGLRenderer & renderer) {
	LoadUniforms();
	if (!depthTest) {
		glDisable(GL_DEPTH_TEST);
	}


	if (mesh != nullptr) {
		mesh->Draw();
	}

	if (!depthTest) {
		glEnable(GL_DEPTH_TEST);
	}
}
