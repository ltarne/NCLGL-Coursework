#pragma once
#include "OGLRenderer.h"
#include "StencilNode.h"
#include "Camera.h"
#include "Frustum.h"
#include "MD5Node.h"
#include "HeightMap.h"
#include "Scene.h"
#include <algorithm>

#define SHADOWSIZE 2048

struct FrameBufferInfo {
	FrameBufferInfo() {
		bufferFBO = 0;
		processFBO = 0;
		shadowTex = 0;
		bufferColourTex[0] = 0;
		bufferColourTex[1] = 0;
		bufferDepthTex = 0;
	}

	GLuint bufferFBO;
	GLuint processFBO;
	GLuint shadowTex;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;
};

class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void RenderScene();

	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }

	inline void SetViewMatrix(Matrix4 viewMatrix) { this->viewMatrix = viewMatrix; }
	inline void SetProjMatrix(Matrix4 projMatrix) { this->projMatrix = projMatrix; }
	inline void SetTextureMatrix(Matrix4 textureMatrix) { this->textureMatrix = textureMatrix; }

	inline void SetActiveScene(Scene* activeScene) { this->activeScene = activeScene; }
	inline void SetOverrideShader(Shader* shader) { this->overrideShader = shader; }

	inline void UpdateGlobalTextures(Shader* shader);

	inline FrameBufferInfo* GetFBInfo() { return &FBInfo; }

protected:
	void LoadPostProcessing();

	void DrawNodes();
	void DrawNode(SceneNode* node);
	
	Shader* overrideShader;

	FrameBufferInfo FBInfo;

	Scene* activeScene;
};
