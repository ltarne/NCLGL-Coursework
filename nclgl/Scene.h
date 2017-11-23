#pragma once
//#include "SceneNode.h"
#include "LightNode.h"
#include "Frustum.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include <algorithm>

enum RenderStages { BASIC_STAGE, SHADOW_STAGE, DEFERRED_LIGHT_STAGE, BLOOM_STAGE, COLOUR_CORRECTION_STAGE, PRESENT_STAGE, TEXT_STAGE, MAX_STAGE };

class Scene
{
public:
	Scene();
	~Scene();

	inline void AddNode(SceneNode* node) { this->root->AddChild(node); }
	inline void AddLight(LightNode* light) { lightList.push_back(light); }
	inline void AddEffect(SceneNode* effect) { nonShadowCastingNodes.push_back(effect); }

	inline SceneNode* GetRoot() { return root; }

	inline vector<SceneNode*>* GetTransparentNodeList() { return &transparentNodeList; }
	inline vector<SceneNode*>* GetNodeList() { return &nodeList; }
	inline vector<LightNode*>* GetLightList() { return &lightList; }

	void SetNonShadowCastingNodesVisibility(bool visible) {
		for (int i = 0; i < nonShadowCastingNodes.size(); ++i) {
			nonShadowCastingNodes[i]->SetVisible(visible);
		}
	}

	inline void SetLight(Light* light) { this->light = light; }

	inline void SetProjMatrix(Matrix4 projMatrix) { this->projMatrix = projMatrix; }

	inline void SetSkyBox(SceneNode* skybox) { this->skybox = skybox; }

	inline Matrix4 GetProjMatrix() { return projMatrix; }
	inline Matrix4 GetViewMatrix() { return viewMatrix; }
	inline Camera* GetCamera() { return camera; }
	inline Light* GetLight() { return light; }

	inline void SetRenderStages(vector<RenderStages> stages) { this->stages = stages; }
	inline vector<RenderStages> GetRenderStages() { return stages; }

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

	SceneNode* skybox;

	Light* light;

	vector<SceneNode*> nonShadowCastingNodes;

	vector<LightNode*> lightList;

	vector<RenderStages> stages;


	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};

