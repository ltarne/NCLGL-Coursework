#pragma once
#include "OGLRenderer.h"
#include "StencilNode.h"
#include "Camera.h"
#include "Frustum.h"
#include "MD5Node.h"
#include "ShadowEffect.h"
#include "HeightMap.h"
#include <algorithm>

#define SHADOWSIZE 2048


class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void SwitchToPerspective();
	void SwitchToOrthographic();

	void ToggleAlphaBlend();
	void ToggleBlendMode();

	void ToggleScissor();
	void ToggleStencil();

	void AttachSceneGraph(SceneNode* node) { root->AddChild(node); }

	void AttachPostProcessingEffect(PostProcessingEffect* effect) {
		postProcessingList.push_back(effect);
		effect->SetFBInfo(&FBInfo);
	}

	void ToggleDepth();
	
	void SetRotation(float rotation) { root->SetRotation(Matrix4::Rotation(rotation, Vector3(0,0,1))); }
	void SetScale(float scale) { root->SetScale(Matrix4::Scale(Vector3(scale,scale,scale))); }
	void SetPosition(Vector3 position) { root->SetTransform(Matrix4::Translation(position)); }

	


protected:
	void LoadPostProcessing();
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();

	void CreateShadowMap();
	void PresentShadows();

	void DrawNodes();
	void DrawNode(SceneNode* node);

	void DrawEffects();
	void PresentScene();
	void DrawScene();

	FrameBufferInfo FBInfo;
	
	Light* light;
	SceneNode* root;
	Camera* camera;
	Frustum frameFrustrum;

	bool usingDepth;
	bool usingAlpha;
	int blendMode;
	bool usingScissor;
	bool usingStencil;
	bool usingShadows;


	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
	vector<PostProcessingEffect*> postProcessingList;
};
