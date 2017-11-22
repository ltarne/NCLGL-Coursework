#pragma once
#include "SceneNode.h"
class TessellatedHeightMapNode :
	public SceneNode
{
public:
	TessellatedHeightMapNode(int tiles, int scale,Shader* shader = nullptr, Mesh* mesh = nullptr, Vector4 colour = Vector4(1, 1, 1, 1));
	~TessellatedHeightMapNode();

	inline virtual void	AddTexture(Texture* texture) { 
		this->textures.push_back(texture); 
		for (int i = 0; i < children.size(); ++i) {
			children[i]->AddTexture(texture);
		}
	}
};

