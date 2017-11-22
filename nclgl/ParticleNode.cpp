#include "ParticleNode.h"


ParticleNode::ParticleNode(Shader * shader, ParticleEmitter* emitter, Vector4 colour) :SceneNode(shader) {
	this->emitter = emitter;
}

ParticleNode::~ParticleNode() {
}

void ParticleNode::LoadUniforms(Shader * shader) {

	//Transform
	Matrix4 modelMatrix = worldTransform * scale;
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);

	//Colour
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "nodeColour"), 1, (float*)&colour);

	//Textures
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "useTexture"), textures.size() > 0 ? true : false);

	for (int i = 0; i < textures.size() && i < TEXTURE_UNIT_MAX; ++i) {
		textures[i]->LoadTexture(shader->GetProgram(), i);
	}
}

void ParticleNode::Update(float msec) {
	if (parent) {
		worldTransform = parent->GetWorldTransform() * (transform * rotation);
	}
	else {
		worldTransform = (transform * rotation);
	}

	emitter->Update(msec);

	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

void ParticleNode::Draw(const OGLRenderer & renderer, Shader * overrideShader) {
	if (overrideShader != nullptr) {
		LoadUniforms(overrideShader);
	}
	else {
		LoadUniforms(shader);
	}


	if (!depthTest) {
		glDisable(GL_DEPTH_TEST);
	}
	if (!faceCulling) {
		glDisable(GL_CULL_FACE);
	}

	glDepthMask(GL_FALSE);


	if (emitter != nullptr) {
		emitter->Draw();
	}

	glDepthMask(GL_TRUE);

	if (!depthTest) {
		glEnable(GL_DEPTH_TEST);
	}
	if (!faceCulling) {
		glEnable(GL_CULL_FACE);
	}
}
