#pragma once
#include "SceneNode.h"
#include "Frustum.h"
#include "Camera.h"
#include <algorithm>

class Scene
{
public:
	Scene();
	~Scene();

	void AttachNode(SceneNode* node) { this->root->AddChild(node); }
	SceneNode* GetRoot() { return root; }

	vector<SceneNode*>* GetTransparentNodeList() { return &transparentNodeList; }
	vector<SceneNode*>* GetNodeList() { return &nodeList; }


	void Update(float msec);

	void BuildNodeLists(SceneNode* from);
	void QuickSortNodeLists();

	void InsertionSortNodeLists();
	void ClearNodeList();

protected:
	Matrix4 projMatrix;
	Matrix4 viewMatrix;

	Frustum frameFrustrum;
	Camera* camera;

	SceneNode* root;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};

