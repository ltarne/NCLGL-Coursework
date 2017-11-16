#pragma once
#include "Scene.h"
#include "Renderer.h"

class RenderStage
{
public:
	RenderStage(Renderer* renderer);
	~RenderStage();

	virtual void DrawStage(Scene* scene);

	static void GenerateQuad() { quad = Mesh::GenerateQuad(); }

	static void DeleteQuad() { delete quad; }

	

protected:
	Renderer* renderer;

	GLuint bufferFBO;

	static Mesh* quad;

};

