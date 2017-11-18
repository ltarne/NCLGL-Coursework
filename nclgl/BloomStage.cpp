#include "BloomStage.h"



BloomStage::BloomStage(Renderer* renderer) : RenderStage(renderer) {
	shader = new Shader(SHADERDIR"bloomVert.vert", SHADERDIR"bloomFrag.frag");
}


BloomStage::~BloomStage() {
	delete shader;
}
