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
	Matrix4 modelMatrix = worldTransform * scale;
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);

	glUniform1i(glGetUniformLocation(shader->GetProgram(), "depthTex"), 9);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "normTex"), 10);

	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "lightPos"), 1, (float*)&position);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "lightColour"), 1, (float*)&colour);
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "lightRadius"), radius);

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
