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

	//GLuint  lightsUBO;
	
	/*static const int maxLights = 10;
	
	LightData lightData[maxLights];*/

	int usedLights;

	GLuint shadowTextures[MAX_LIGHTS] = { 0 };
	Vector4 lightColours[MAX_LIGHTS] = { Vector4() };
	Vector3 lightPositions[MAX_LIGHTS] = { Vector3() };
	float lightRadius[MAX_LIGHTS] = { 0.0f };
	Matrix4 lightTextureMatrix[MAX_LIGHTS] = { Matrix4() };
	
	

	GLuint shadowTex;
};

