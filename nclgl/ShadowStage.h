#pragma once
#include "RenderStage.h"
struct LightData {
	Vector3 lightPosition;
	Vector3 lightColour;
	Vector3 lightRadius;
};

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
	LightData lightData[10];

	GLuint shadowTex;
};

