#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	camera = new Camera();

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 0, 2));

	root = new SceneNode(debugDrawShader);

	light = new Light(Vector3((RAW_WIDTH*HEIGHTMAP_X) / 2, 500.0f, -(RAW_HEIGHT*HEIGHTMAP_Z)/2),
		Vector4(1,1.0,1.0,1), (RAW_WIDTH*HEIGHTMAP_X)*2.0f);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	LoadPostProcessing();

	init = true;

	usingDepth = true;
	usingAlpha = false;
	blendMode = 0;
	usingScissor = false;
	usingStencil = false;
	usingShadows = false;
}
Renderer::~Renderer(void)	{
	delete light;
	delete root;
	delete camera;
	PostProcessingEffect::DeleteQuad();

	glDeleteTextures(2, FBInfo.bufferColourTex);
	glDeleteTextures(1, &FBInfo.bufferDepthTex);
	glDeleteFramebuffers(1, &FBInfo.bufferFBO);
	glDeleteFramebuffers(1, &FBInfo.processFBO);
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustrum.FromMatrix(projMatrix * viewMatrix);

	root->Update(msec);
}

void Renderer::RenderScene()	{
	if (usingShadows) {

	}
	else if (postProcessingList.size() > 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, FBInfo.bufferFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBInfo.bufferColourTex[0], 0);
		DrawScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		DrawEffects();
		PresentScene();
	}
	else {
		DrawScene();
	}
	SwapBuffers();
}


void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}

void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}

void Renderer::ToggleAlphaBlend() {
	usingAlpha = !usingAlpha;
	usingAlpha ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void Renderer::ToggleBlendMode() {
	blendMode = (blendMode + 1) % 4;

	switch (blendMode) {
	case(0):
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case(1):
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
		break;

	case(2):
		glBlendFunc(GL_ONE, GL_ZERO);
		break;

	case(3):
		glBlendFunc(GL_SRC_ALPHA, GL_ALPHA);
		break;
	}
}

void Renderer::ToggleScissor() {
	usingScissor = !usingScissor;
	usingScissor ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);

}

void Renderer::ToggleStencil() {
	usingStencil = !usingStencil;
	usingStencil ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
}

void Renderer::ToggleDepth() {
	usingDepth = !usingDepth;
	usingDepth ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
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

void Renderer::BuildNodeLists(SceneNode * from) {
	if (frameFrustrum.InsideFrustrum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();

		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}
	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void Renderer::SortNodeLists() {
	sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);

	sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::CreateShadowMap() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBInfo.bufferFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	//ChangeShader

	viewMatrix = postProcessingList[0]->GetViewMatrix();
	textureMatrix = biasMatrix * (projMatrix * viewMatrix);

	DrawScene();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::PresentShadows() {
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, FBInfo.bufferColourTex[0]);
}

void Renderer::DrawNodes() {
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i) {
		DrawNode((*i));
	}

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i) {
		DrawNode((*i));
	}
}

void Renderer::DrawNode(SceneNode* node) {
	

	if (node->GetVisible() && node->GetMesh()) {
		glUseProgram(node->GetShader()->GetProgram());
		UpdateShaderMatrices(node->GetShader());

		if (light) {
			SetShaderLight(*light, node->GetShader());
			glUniform3fv(glGetUniformLocation(node->GetShader()->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
		}

		node->Draw(*this);

		glUseProgram(0);
	}

	

	/*for (vector<SceneNode*>::const_iterator i = node->GetChildIteratorStart(); i != node->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}*/
}

void Renderer::DrawEffects() {
	for (int i = 0; i < postProcessingList.size(); ++i) {
		postProcessingList[i]->DrawOnce();
	}
}

void Renderer::PresentScene() {
	postProcessingList.back()->Present();
}

void Renderer::DrawScene() {
	
	BuildNodeLists(root);
	SortNodeLists();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glScissor(0, 0, width, height);//Sets the scissor region to the whole screen before the clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (usingScissor) {

		glScissor((float)width / 2.5f, (float)height / 2.5f,
			(float)width / 5.0f, (float)height / 5.0f);
	}


	DrawNodes();

	ClearNodeLists();

	
}

