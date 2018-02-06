#pragma once
#include "Scene.h"
#include "Renderer.h"

class RenderStage
{
public:
	RenderStage(Renderer* renderer);
	virtual ~RenderStage();

	virtual void DrawStage(Scene* scene);

	static void GenerateQuad() { quad = Mesh::GenerateQuad(); }

	void DrawCombineQuad();

	static void DeleteQuad() { delete quad; }

	

protected:
	Renderer* renderer;

	Shader* stageShader;
	Matrix4 projMatrix;

	GLuint bufferFBO;
	GLuint* colourBuffers;
	GLuint bufferDepthTex;

	static Mesh* quad;

};

