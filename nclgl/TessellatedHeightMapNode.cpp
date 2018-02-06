#include "TessellatedHeightMapNode.h"


TessellatedHeightMapNode::TessellatedHeightMapNode(int tiles, int scale, Shader * shader, Mesh * mesh, Vector4 colour) {
	int line = sqrt(tiles);
	for (int x = 0; x < line; ++x) {
		for (int z = 0; z < line; ++z) {
			SceneNode* tile = new SceneNode(shader, mesh);
			tile->SetTransform(Matrix4::Translation(Vector3(x*(scale * 2), 0, z * (scale * 2))));
			tile->SetRotation(Matrix4::Rotation(-90, Vector3(1, 0, 0)));
			tile->SetScale(Vector3(scale,scale,scale));
			tile->SetBoundingRadius(scale *2);
			
			AddChild(tile);
		}
	}
}

TessellatedHeightMapNode::~TessellatedHeightMapNode() {
}

void TessellatedHeightMapNode::Update(float msec) {
	if (parent) {
		worldTransform = parent->GetWorldTransform() * (transform * rotation);
	}
	else {
		worldTransform = (transform * rotation);
	}

	time += msec;

	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

void TessellatedHeightMapNode::LoadUniforms(Shader * shader) {
	//Transform
	Matrix4 modelMatrix = worldTransform * scale;
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);

	//Colour
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "nodeColour"), 1, (float*)&colour);

	glUniform1f(glGetUniformLocation(shader->GetProgram(), "msec"), time);

	//Textures
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "useTexture"), textures.size() > 0 ? true : false);



	for (int i = 0; i < textures.size() && i < TEXTURE_UNIT_MAX; ++i) {
		textures[i]->LoadTexture(shader->GetProgram(), i);
	}
}
