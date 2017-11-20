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
	void DettachFBOData();

	virtual void DrawStage(Scene* scene);

protected:
	void FillBuffers();
	void DrawPointLights(Scene* scene);
	void CombineBuffers(Scene* scene);

	void GenerateScreenTexture(GLuint &into, bool depth = false);

	Shader* combineShader;

	GLuint geometryFBO;
	GLuint pointLightFBO;

	GLuint bufferDepthTex;
	GLuint bufferColourTex;
	GLuint bufferNormalTex;
	GLuint bufferEmissiveTex;
	GLuint bufferSpecularTex;

};

