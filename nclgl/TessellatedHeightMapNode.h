#pragma once
#include "SceneNode.h"
class TessellatedHeightMapNode :
	public SceneNode
{
public:
	TessellatedHeightMapNode(int tiles, int scale,Shader* shader = nullptr, Mesh* mesh = nullptr, Vector4 colour = Vector4(1, 1, 1, 1));
	~TessellatedHeightMapNode();
};

