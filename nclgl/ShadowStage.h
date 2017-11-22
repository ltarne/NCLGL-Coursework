#pragma once
#include "RenderStage.h"


class ShadowStage :
	public RenderStage
{
public:
	ShadowStage(Renderer* renderer);
	~ShadowStage();

	virtual void DrawStage(Scene* scene);

protected:
	void CreateShadowTextures(Scene * scene);
	void PresentScene(Scene * scene);

	GLuint shadowFBO;

	GLuint  lightsUBO;
	vector<GLuint> shadowTextures;
	static const int maxLights = 10;
	int usedLights;
	LightData lightData[maxLights];
	

	GLuint shadowTex;
};

