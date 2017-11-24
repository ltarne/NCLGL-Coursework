#include "SceneNode.h"


SceneNode::SceneNode(Shader* shader, Mesh * mesh, Vector4 colour) {
	this->shader	= shader;
	this->mesh		= mesh;
	this->colour	= colour;

	parent			= NULL;
	transform	= Matrix4();
	rotation	= Matrix4::Rotation(0.0f, Vector3(0,0,0));
	scale		= Matrix4::Scale(Vector3(1, 1, 1));

	visible = true;

	time = 0;

	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;

	depthTest = true;
	faceCulling = true;
}

SceneNode::~SceneNode() {
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::LoadUniforms(Shader* shader) {	

	//Transform
	Matrix4 modelMatrix = worldTransform * scale;
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);

	//Colour
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "nodeColour"), 1, (float*)&colour);

	glUniform1f(glGetUniformLocation(shader->GetProgram(), "msec"), time);

	glUniform1i(glGetUniformLocation(shader->GetProgram(), "skeleton"), false);

	//Textures
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "useTexture"), textures.size() > 0 ? true : false);

	for (int i = 0; i < textures.size() && i < TEXTURE_UNIT_MAX; ++i) {
		textures[i]->LoadTexture(shader->GetProgram(), i);
	}
}

void SceneNode::AddChild(SceneNode* child) {
	children.push_back(child);
	child->parent = this;
}

void SceneNode::Update(float msec) {
	if (parent) {
		worldTransform = parent->worldTransform * (transform * rotation);
	}
	else {
		worldTransform = (transform * rotation);
	}

	time += msec;

	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

void SceneNode::Draw(const OGLRenderer &renderer, Shader* overrideShader) {

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
	

	if (mesh != nullptr) {
		mesh->Draw();
	}

	if (!depthTest) {
		glEnable(GL_DEPTH_TEST);
	}
	if (!faceCulling) {
		glEnable(GL_CULL_FACE);
	}
}
