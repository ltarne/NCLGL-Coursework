#pragma once
#include "SceneNode.h"

class Scene
{
public:
	Scene();
	~Scene();

	void AttachNode(SceneNode* node) { this->root->AddChild(node); }


	void BuildNodeList();
	void UpdateNodeList();
	void ClearNodeList();

protected:
	SceneNode* root;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};

