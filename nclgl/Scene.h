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

	inline void SetLight(Light* light) { this->light = light; }

	inline void SetProjMatrix(Matrix4 projMatrix) { this->projMatrix = projMatrix; }

	inline Matrix4 GetProjMatrix() { return projMatrix; }
	inline Matrix4 GetViewMatrix() { return viewMatrix; }
	inline Camera* GetCamera() { return camera; }
	inline Light* GetLight() { return light; }

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

	Light* light;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};

