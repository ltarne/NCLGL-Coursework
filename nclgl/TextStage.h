#pragma once
#include "RenderStage.h"
class TextStage :
	public RenderStage {
public:
	TextStage(Renderer* renderer);
	~TextStage();

	virtual void DrawStage(Scene* scene);

	void WriteToScreen(string message, Vector3 position, int size);

	inline void ToggleBloom() { bloom = !bloom; }
	inline void ToggleGreen() { green = !green; }

	inline void TogglePaused() { paused = !paused; }

protected:
	Font* basicFont;
	Shader* stageShader;

	bool bloom = false;
	bool green = false;

	bool paused = false;

	Matrix4 projMatrix;
	Matrix4 viewMatrix;
	Matrix4 modelMatrix;
};

