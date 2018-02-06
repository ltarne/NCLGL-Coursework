#pragma once
#include "Renderer.h"
#include "ShadowStage.h"
#include "DefereredLightsStage.h"
#include "ColourCorrectionStage.h"
#include "PresentStage.h"
#include "BloomStage.h"
#include "TextStage.h"



class RenderingManager
{
public:
	RenderingManager(Renderer* renderer);
	~RenderingManager();

	virtual void UpdateScene(float msec);
	virtual void DrawScene();

	inline void SetActiveScene(Scene* activeScene) { 
		this->activeScene = activeScene; 
		renderer->SetActiveScene(this->activeScene);
	}

	inline void TogglePause() { textStage->TogglePaused(); }

	inline map<RenderStages, bool>* GetPostProcessingSelect() { return &postProcessingSelect; }


	inline void TogglePostProcessingEffect(RenderStages process) {
		postProcessingSelect.find(process)->second = !postProcessingSelect.find(process)->second;
		if (process == BLOOM_STAGE) {
			textStage->ToggleBloom();
		}
		else if (process == COLOUR_CORRECTION_STAGE) {
			textStage->ToggleGreen();
		}
	}


protected:
	Renderer*	 renderer;
	RenderStage* renderStages[MAX_STAGE];

	map<RenderStages, bool> postProcessingSelect = { {BLOOM_STAGE, false}, {COLOUR_CORRECTION_STAGE, false} };
	
	TextStage* textStage;
	Scene* activeScene;

	
	
};

