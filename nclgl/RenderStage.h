#pragma once
class RenderStage
{
public:
	RenderStage();
	~RenderStage();

	virtual void DrawStage(Scene* scene);
};

