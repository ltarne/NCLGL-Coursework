#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	light = new Light(Vector3((RAW_WIDTH*HEIGHTMAP_X) / 2, 500.0f, -(RAW_HEIGHT*HEIGHTMAP_Z)/2),
		Vector4(1,1.0,1.0,1), (RAW_WIDTH*HEIGHTMAP_X)*2.0f);

	LoadPostProcessing();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	init = true;
}
Renderer::~Renderer(void)	{
	delete light;

	glDeleteTextures(2, FBInfo.bufferColourTex);
	glDeleteTextures(1, &FBInfo.bufferDepthTex);
	glDeleteFramebuffers(1, &FBInfo.bufferFBO);
	glDeleteFramebuffers(1, &FBInfo.processFBO);
}



void Renderer::LoadPostProcessing() {
	glGenTextures(1, &FBInfo.bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, FBInfo.bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &FBInfo.bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, FBInfo.bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glGenFramebuffers(1, &FBInfo.bufferFBO); //Render scene into this
	glGenFramebuffers(1, &FBInfo.processFBO); //Post processing in this

	glBindFramebuffer(GL_FRAMEBUFFER, FBInfo.bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FBInfo.bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, FBInfo.bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBInfo.bufferColourTex[0], 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !FBInfo.bufferDepthTex || !FBInfo.bufferColourTex[0]) {
		cout << "Framebuffer failed!\n";
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}





void Renderer::DrawNodes() {
	for (vector<SceneNode*>::const_iterator i = nodeList->begin(); i != nodeList->end(); ++i) {
		DrawNode((*i));
	}

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList->rbegin(); i != transparentNodeList->rend(); ++i) {
		DrawNode((*i));
	}
}

void Renderer::DrawNode(SceneNode* node) {
	

	if (node->GetVisible() && node->GetMesh()) {
		glUseProgram(node->GetShader()->GetProgram());
		UpdateShaderMatrices(node->GetShader());

		/*if (light) {
			SetShaderLight(*light, node->GetShader());
			glUniform3fv(glGetUniformLocation(node->GetShader()->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
		}*/

		node->Draw(*this);

		glUseProgram(0);
	}

}


void Renderer::RenderScene() {


	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glScissor(0, 0, width, height);//Sets the scissor region to the whole screen before the clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//if (usingScissor) {

	//	glScissor((float)width / 2.5f, (float)height / 2.5f,
	//		(float)width / 5.0f, (float)height / 5.0f);
	//}


	DrawNodes();
	
}

