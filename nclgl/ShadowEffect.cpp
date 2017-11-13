#include "ShadowEffect.h"



ShadowEffect::ShadowEffect(int width, int height, Shader* sceneShader, Shader* processingShader) 
	: PostProcessingEffect(width, height, sceneShader, processingShader) {
}


ShadowEffect::~ShadowEffect() {
}
