#pragma once
#include"Shader.h"

#define POST_PASSES 10

struct FrameBufferInfo {
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;
};

class PostProcessingEffect
{
public:
	PostProcessingEffect(int width, int height, Shader* sceneShader, Shader* processingShader);
	~PostProcessingEffect();

	inline void SetFBInfo(FrameBufferInfo* FBInfo) { this->FBInfo = FBInfo; }

	inline static void GenerateQuad() {
		quad = Mesh::GenerateQuad();
	}

	inline static void DeleteQuad() {
		if (quad) {
			delete quad;
		}
		
	}

	inline void SetViewMatrix(Matrix4 viewMatrix) { this->viewMatrix = viewMatrix; }
	inline Matrix4 GetViewMatrix() { return viewMatrix; }

	virtual void Draw();
	virtual void DrawOnce();

	virtual void Present();

	void UpdateShaderMatrices(Shader* shader);

protected:
	Shader* sceneShader;
	Shader* processShader;

	FrameBufferInfo* FBInfo;

	Matrix4 modelMatrix;
	Matrix4 projMatrix;
	Matrix4 viewMatrix;

	int width;
	int height;

	static Mesh* quad;
};

