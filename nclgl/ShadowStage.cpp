#include "ShadowStage.h"



ShadowStage::ShadowStage(Renderer* renderer) 
	: RenderStage(renderer){
	stageShader = new Shader(SHADERDIR"skeletonShadowMapVert.vert", SHADERDIR"shadowFrag.frag");
	stageShader->LinkProgram();

	usedLights = 0;

	glGenFramebuffers(1, &shadowFBO);

	for (int i = 0; i < MAX_LIGHTS; ++i) {
		glGenTextures(1, &shadowTextures[i]);
		glBindTexture(GL_TEXTURE_2D, shadowTextures[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//glGenTextures(1, &shadowTex);
	//glBindTexture(GL_TEXTURE_2D, shadowTex);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*glGenBuffers(1, &lightsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lightData), &lightData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	renderer->SetLightUBO(&lightsUBO);*/


}


ShadowStage::~ShadowStage() {
}

void ShadowStage::DrawStage(Scene * scene) {

	renderer->SetViewMatrix(scene->GetViewMatrix());
	renderer->SetProjMatrix(scene->GetProjMatrix());
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	scene->SetNonShadowCastingNodesVisibility(false);
	CreateShadowTextures(scene);
	scene->SetNonShadowCastingNodesVisibility(true);
	renderer->SetUsingShadows(true);
	PresentScene(scene);
	renderer->SetUsingShadows(false);

}

void ShadowStage::CreateShadowTextures(Scene * scene) {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(1, 1, SHADOWSIZE-2, SHADOWSIZE-2);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	vector<LightNode*> lights = *scene->GetLightList();
	

	for (int i = 0; i < lights.size() && i < MAX_LIGHTS; ++i) {
		lights[i]->SetShader(stageShader);
		usedLights++;

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTextures[i], 0);

		glClear(GL_DEPTH_BUFFER_BIT);
		Vector3 point = lights[i]->GetDirection() /** lights[i]->GetRadius()*/;

		renderer->SetViewMatrix(Matrix4::BuildViewMatrix(lights[i]->GetPosition(), point));

		//renderer->SetTextureMatrix(biasMatrix*(scene->GetProjMatrix() * Matrix4::BuildViewMatrix(lights[i]->GetPosition(), point)));

		lightTextureMatrix[i] = (biasMatrix*(scene->GetProjMatrix() * Matrix4::BuildViewMatrix(lights[i]->GetPosition(), point)));
		lightColours[i] = lights[i]->GetColour();
		lightPositions[i] = lights[i]->GetPosition();
		lightRadius[i] = lights[i]->GetRadius();


		renderer->DrawNodes(stageShader);
	}

	/*LightNode* light = (*scene->GetLightList())[0];
	light->SetShader(stageShader);
	

	renderer->SetViewMatrix(Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, -1000)));
	
	renderer->SetTextureMatrix(biasMatrix*(scene->GetProjMatrix() * Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, -1000))));

	

	renderer->DrawNodes(stageShader);*/


	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, renderer->GetWidth(), renderer->GetHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ShadowStage::PresentScene(Scene * scene) {
	/*glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, shadowTex);*/
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	
	
	renderer->SetUsedLights(usedLights);
	renderer->SetLightColours(lightColours);
	renderer->SetLightPositions(lightPositions);
	renderer->SetLightRadius(lightRadius);
	renderer->SetLightTextureMatrix(lightTextureMatrix);

	renderer->SetShadowTextures(shadowTextures);
	
	//(*scene->GetRoot()->GetChildIteratorStart())->GetTextures()->back()->SetTexture(shadowTex);

	renderer->SetViewMatrix(scene->GetCamera()->BuildViewMatrix());

	renderer->RenderScene();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	usedLights = 0;
}
