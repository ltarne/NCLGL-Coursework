#include "Renderer.h"

Renderer::Renderer(Window &parent, int* fps) : OGLRenderer(parent)	{
	//projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	LoadPostProcessing();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	this->fps = fps;

	usingShadows = false;

	init = true;
}
Renderer::~Renderer(void)	{


	glDeleteFramebuffers(1, &bufferFBO);

}



inline void Renderer::UpdateGlobalTextures(Shader* shader) {
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "shadowTex"), 9);
}

void Renderer::LoadPostProcessing() {


	glGenFramebuffers(1, &bufferFBO); //Render scene into this

	GLuint bufferAttachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glGenTextures(2, colourBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);

	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);


	for (int i = 0; i < 2; ++i) {
		glBindTexture(GL_TEXTURE_2D, colourBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, bufferAttachments[i], GL_TEXTURE_2D, colourBuffers[i], 0);
		
	}
	glDrawBuffers(2, bufferAttachments);
	
	

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "Framebuffer failed!\n";
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}





void Renderer::DrawLights() {
	vector<LightNode*> lightList = *activeScene->GetLightList();
	for (vector<LightNode*>::const_iterator i = lightList.begin(); i != lightList.end(); ++i) {
		LightNode* temp = *i;
		float dist = (temp->GetPosition() - activeScene->GetCamera()->GetPosition()).Length();
		if (dist < temp->GetRadius()) {
			glCullFace(GL_FRONT);
		}
		else {
			glCullFace(GL_BACK);
		}
		
		temp->SetRotation(temp->GetRotation() * Matrix4::Rotation(0.5f, Vector3(0, 1, 0)));
		DrawNode(*i);
	}
}

void Renderer::DrawNodes(Shader* overrideShader) {
	vector<SceneNode*> nodeList = *activeScene->GetNodeList();
	vector<SceneNode*> transparentNodeList = *activeScene->GetTransparentNodeList();
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i) {
		DrawNode((*i),overrideShader);
	}

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i) 
	{
		DrawNode((*i), overrideShader);
	}
}

void Renderer::DrawNode(SceneNode* node, Shader* overrideShader) {

	Shader* activeShader = overrideShader != nullptr ? overrideShader : node->GetShader();
	

	if (node->GetVisible() && node->GetShader()) {
		//node->SetOverrideShader(overrideShader);

		glUseProgram(activeShader->GetProgram());
		//viewMatrix = activeScene->GetViewMatrix();
		UpdateShaderMatrices(activeShader);
		

		if (usingShadows) {
			UpdateGlobalTextures(activeShader);
			LightNode* light = (*activeScene->GetLightList())[0];
			glUniform3fv(glGetUniformLocation(activeShader->GetProgram(), "lightPos"), 1, (float*)&light->GetPosition());
			glUniform4fv(glGetUniformLocation(activeShader->GetProgram(), "lightColour"), 1, (float*)&light->GetColour());
			glUniform1f(glGetUniformLocation(activeShader->GetProgram(), "lightRadius"), light->GetRadius());
		}
		glUniform3fv(glGetUniformLocation(activeShader->GetProgram(), "cameraPos"), 1, (float*)&activeScene->GetCamera()->GetPosition());
		glUniform2f(glGetUniformLocation(activeShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

		node->Draw(*this, overrideShader);

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


void Renderer::DrawText(const std::string & text,  Font* basicFont) {
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->GetTexture());

	
	mesh->Draw();

	delete mesh; //Once it's drawn, we don't need it anymore!
}

