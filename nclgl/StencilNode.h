#pragma once
#include "SceneNode.h"
class StencilNode :
	public SceneNode
{
public:
	StencilNode(Shader* shader, Mesh* mesh = nullptr, Vector4 colour = Vector4(1, 1, 1, 1));
	~StencilNode();

	virtual void Draw(const OGLRenderer &renderer);
};

