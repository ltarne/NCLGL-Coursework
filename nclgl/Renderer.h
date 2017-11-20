#pragma once
#include "OGLRenderer.h"
#include "StencilNode.h"
#include "Camera.h"
#include "Frustum.h"
#include "MD5Node.h"
#include "HeightMap.h"
#include "Scene.h"
#include "TextMesh.h"
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
	Renderer(Window &parent, int* fps);
	virtual ~Renderer(void);

	virtual void RenderScene();

	inline void UseOrthographic() {
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	inline void UseProjection() {
		projMatrix = Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f);
	}

	void DrawText(const std::string &text, Font* basicFont);

	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }

	inline int GetFPS() { return *fps; }

	inline void SetViewMatrix(Matrix4 viewMatrix) { this->viewMatrix = viewMatrix; }
	inline void SetProjMatrix(Matrix4 projMatrix) { this->projMatrix = projMatrix; }
	inline void SetTextureMatrix(Matrix4 textureMatrix) { this->textureMatrix = textureMatrix; }

	inline void SetActiveScene(Scene* activeScene) { this->activeScene = activeScene; }
	inline void SetOverrideShader(Shader* shader) { this->overrideShader = shader; }

	inline void UpdateGlobalTextures(Shader* shader);

	inline GLuint GetBufferFBO() { return bufferFBO; }
	inline GLuint* GetColourBuffers() { return colourBuffers; }

	inline void SetUsingShadows(bool usingShadows) { this->usingShadows = usingShadows; }
	inline bool GetUsingShadows() { return usingShadows; }

	void DrawLights();
protected:
	void LoadPostProcessing();

	
	void DrawNodes();
	void DrawNode(SceneNode* node);
	
	Shader* overrideShader;

	bool usingShadows;

	GLuint bufferFBO;
	GLuint colourBuffers[2];

	Scene* activeScene;

	int* fps;
};
