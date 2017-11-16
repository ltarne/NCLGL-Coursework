#pragma once
#include "RenderStage.h"

//#define LIGHTNUM 8


class DefereredLightsStage :
	public RenderStage
{
public:
	DefereredLightsStage(Renderer* renderer);
	~DefereredLightsStage();

	void AttachFBOData();

	virtual void DrawStage(Scene* scene);

protected:
	void FillBuffers();
	void DrawPointLights();
	void CombineBuffers(Scene* scene);

	void GenerateScreenTexture(GLuint &into, bool depth = false);

	Matrix4 projMatrix;

	Shader* combineShader;

	GLuint pointLightFBO;

	GLuint bufferDepthTex;
	GLuint bufferColourTex;
	GLuint bufferNormalTex;
	GLuint bufferEmissiveTex;
	GLuint bufferSpecularTex;

};

