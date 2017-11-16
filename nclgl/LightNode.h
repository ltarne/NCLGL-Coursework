#pragma once
#include "SceneNode.h"
class LightNode :
	public SceneNode
{
public:
	LightNode(Shader* shader = nullptr, Mesh* mesh = nullptr, Vector4 colour = Vector4(1, 1, 1, 1), float radius = 10.0f, Vector3 pos = Vector3(0,0,0));
	~LightNode();

	virtual void LoadUniforms();

	virtual void Draw(const OGLRenderer &renderer);

	void SetPosition(Vector3 pos) { 
		transform = Matrix4::Translation(pos);
		position = pos;
	}

protected:
	Vector3 position;
	float radius;
};

