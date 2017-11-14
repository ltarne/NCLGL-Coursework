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

	virtual void RenderScene();

	inline void LoadLists(vector<SceneNode*>* transparentNodeList, vector<SceneNode*>* nodeList) {
		this->transparentNodeList = transparentNodeList;
		this->nodeList = nodeList;
	}
	

protected:
	void LoadPostProcessing();

	void DrawNodes();
	void DrawNode(SceneNode* node);
	

	FrameBufferInfo FBInfo;
	
	Light* light;

	vector<SceneNode*>* transparentNodeList;
	vector<SceneNode*>* nodeList;
};
