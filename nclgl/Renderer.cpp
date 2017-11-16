#include "Renderer.h"

Renderer::Renderer(Window &parent, int* fps) : OGLRenderer(parent)	{
	//projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	LoadPostProcessing();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	overrideShader = nullptr;
	this->fps = fps;

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

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);

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
		DrawNode(*i);
	}
}

void Renderer::DrawNodes() {
	vector<SceneNode*> nodeList = *activeScene->GetNodeList();
	vector<SceneNode*> transparentNodeList = *activeScene->GetTransparentNodeList();
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i) {
		DrawNode((*i));
	}

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i) {
		DrawNode((*i));
	}
}

void Renderer::DrawNode(SceneNode* node) {
	

	if (node->GetVisible() && node->GetMesh()) {
		node->SetOverrideShader(overrideShader);

		glUseProgram(node->GetShader()->GetProgram());
		UpdateShaderMatrices(node->GetShader());
		UpdateGlobalTextures(node->GetShader());

		/*if (activeScene->GetLight()) {
			SetShaderLight(*activeScene->GetLight(), node->GetShader());
			
		}*/
		glUniform3fv(glGetUniformLocation(node->GetShader()->GetProgram(), "cameraPos"), 1, (float*)&activeScene->GetCamera()->GetPosition());
		glUniform2f(glGetUniformLocation(node->GetShader()->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

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


void Renderer::DrawText(const std::string & text,  Font* basicFont) {
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->GetTexture());

	
	mesh->Draw();

	delete mesh; //Once it's drawn, we don't need it anymore!
}

