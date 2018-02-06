#include "SceneLoader.h"



SceneLoader::SceneLoader() {
}


SceneLoader::~SceneLoader() {
	while (shaders.size() > 0) {
		delete shaders.back();
		shaders.pop_back();
	}
	while (meshes.size() > 0) {
		delete meshes.back();
		meshes.pop_back();
	}
	while (textures.size() > 0) {
		delete textures.back();
		textures.pop_back();
	}
}

Scene* SceneLoader::LoadScene1() {
	Scene* scene = new Scene();

	float scale = 1000.0f;

	/* Shaders */

	Shader* shader = new Shader(SHADERDIR"heightMapShadowVert.vert", SHADERDIR"heightMapMultiFrag.frag", "", SHADERDIR"heightMapTCS.tesc", SHADERDIR"heightMapTES.tese");
	if (!shader->LinkProgram()) {
		return NULL;
	}
	shaders.push_back(shader);


	Shader* skyBoxShader = new Shader(SHADERDIR"skyboxVert.vert", SHADERDIR"skyboxFrag.frag");
	skyBoxShader->LinkProgram();
	shaders.push_back(skyBoxShader);

	Shader* reflectionShader = new Shader(SHADERDIR"heightMapShadowVert.vert", SHADERDIR"reflectFrag.frag", "", SHADERDIR"heightMapTCS.tesc", SHADERDIR"waterTES.tese");
	reflectionShader->LinkProgram();
	shaders.push_back(reflectionShader);

	Shader* particleShader = new Shader(SHADERDIR"particleVert.vert", SHADERDIR"particleFrag.frag", SHADERDIR"particleGeom.geom");
	particleShader->LinkProgram();
	shaders.push_back(particleShader);



	/* Meshes */
	Mesh* quad = Mesh::GenerateQuad();
	meshes.push_back(quad);

	Mesh* patch = Mesh::GenerateQuadPatch();
	meshes.push_back(patch);


	ParticleEmitter* rain = new ParticleEmitter();
	rain->SetParticleSize(20.0f);
	rain->SetParticleVariance(1.0f);
	rain->SetLaunchParticles(160.0f);
	rain->SetParticleLifetime(3500.0f);
	rain->SetParticleSpeed(0.7f);
	rain->SetDirection(Vector3(0, -1, 0));
	rain->SetOrigin(Vector3((scale * 2) * 2.0, 2500, (scale * 2) * 2.0));
	rain->SetSpawnWidth((scale*2) * 5);
	rain->SetColourShift(Vector4(0.0, 0.4, 0.5, 1.0));
	meshes.push_back(rain);

	/* Textures */
	//Source: http://www.custommapmakers.org/skyboxes.php
	string skyTextures[6] = { TEXTUREDIR "rusted_west.JPG",TEXTUREDIR "rusted_east.JPG",TEXTUREDIR "rusted_up.JPG",TEXTUREDIR "rusted_down.JPG",TEXTUREDIR "rusted_south.JPG",TEXTUREDIR "rusted_north.JPG" };
	CubeMapTexture* skyCubeMap = new CubeMapTexture(skyTextures, "cubeTex");
	textures.push_back(skyCubeMap);

	Texture* bumpTex = new Texture(TEXTUREDIR"Barren RedsDOT3.JPG", "bumpTex");
	bumpTex->ToggleRepeating();
	textures.push_back(bumpTex);

	//Source: https://opengameart.org/content/synthetic-grass-texture-pack
	Texture* grass = new Texture(TEXTUREDIR"grass.png", "grassTex");
	grass->ToggleRepeating();
	textures.push_back(grass);

	//Source: https://opengameart.org/content/texture-water
	Texture* waterTex = new Texture(TEXTUREDIR"water.png", "waterTex");
	waterTex->ToggleRepeating();
	textures.push_back(waterTex);

	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG", "tex");
	tex->ToggleRepeating();
	textures.push_back(tex);

	//Source: https://www.google.co.uk/search?sa=G&hl=en-GB&tbs=simg:CAESsgIJv_1ECu-SN8OUapgILEKjU2AQaCAgVCAEICQgKDAsQsIynCBpiCmAIAxIo8QbOEr0B7hK8As8SxgK_1Ap8HoAfdKKU0qTb_1M6Y2kimlNuY2oTTnNhowGapVFol079i-7KZwTf3thWyPKYGoChtjFbqYCE6PwSPa9oCiQKW69YCHfPT-aZ4RIAQMCxCOrv4IGgoKCAgBEgRHYH9oDAsQne3BCRqNAQoWCgRkcm9w2qWI9gMKCggvbS8wYjd6MQoXCgRjb25l2qWI9gMLCgkvbS8wM2J6cDAKHgoLY29iYWx0IGJsdWXapYj2AwsKCS9tLzA0ZjYyegoXCgV3YXRlctqliPYDCgoIL20vMDgzOGYKIQoNZWxlY3RyaWMgYmx1ZdqliPYDDAoKL20vMDJ2d2J6egw&q=drop&tbm=isch&ved=0ahUKEwjWjo6MkNbXAhVoCcAKHb7CC6AQsw4INQ&biw=1920&bih=974#imgrc=Knh3dgR9AoJa8M:
	Texture* rainTex = new Texture(TEXTUREDIR"rain.png", "diffuseTex");
	textures.push_back(rainTex);

	Texture* colourTex = new Texture("depthTex");
	textures.push_back(colourTex);

	Texture* normalTex = new Texture("normTex");
	textures.push_back(normalTex);

	/* Scene Nodes */

	SceneNode* skybox = new SceneNode(skyBoxShader, quad);
	skybox->SetDepthTest(false);
	skybox->AddTexture(skyCubeMap);
	skybox->SetBoundingRadius(1000000000.0f);
	skybox->SetFaceCulling(false);

	ParticleNode* rainEmitter = new ParticleNode(particleShader, rain);
	rainEmitter->AddTexture(rainTex);
	rainEmitter->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	rainEmitter->SetBoundingRadius(100000000.0f);
	rainEmitter->SetColour(Vector4(1.0, 1.0, 1.0, 0.5));


	TessellatedHeightMapNode* heightMapRoot = new TessellatedHeightMapNode(16, 1000, shader, patch);
	heightMapRoot->AddTexture(tex);
	heightMapRoot->AddTexture(bumpTex);
	heightMapRoot->AddTexture(grass);

	TessellatedHeightMapNode* water = new TessellatedHeightMapNode(16, scale, reflectionShader, patch);
	water->SetTransform(Matrix4::Translation(Vector3(0, 700, 0)));
	water->AddTexture(waterTex);
	water->AddTexture(skyCubeMap);


	scene->AddNonShadowCastingNode(rainEmitter);

	scene->AddNode(rainEmitter);
	scene->SetSkyBox(skybox);
	scene->AddNode(heightMapRoot);
	scene->AddNode(water);

	


	vector<RenderStages> stages = { BASIC_STAGE };
	scene->SetRenderStages(stages);

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	scene->GetCamera()->SetPosition(Vector3(scale*2, 1800, scale * 2));
	scene->GetCamera()->SetPitch(-15);

	scenes[0];

	return scene;
}


Scene* SceneLoader::LoadScene2() {
	Scene* scene = new Scene();

	Shader* basicShader = new Shader(SHADERDIR"texturedVertex.glsl", SHADERDIR"texturedFragment.glsl");
	basicShader->LinkProgram();
	/* Shaders */
	Shader* shader = new Shader(SHADERDIR"shadowSceneVert.vert", SHADERDIR"shadowSceneFrag.frag");
	shader->LinkProgram();
	shaders.push_back(shader);

	Shader* hellKnightShader = new Shader(SHADERDIR"skeletonShadowVert.vert", SHADERDIR"shadowSceneFrag.frag");
	hellKnightShader->LinkProgram();
	shaders.push_back(hellKnightShader);

	Shader* shaderNoBump = new Shader(SHADERDIR"shadowSceneVert.vert", SHADERDIR"shadowSceneFragNoBump.frag");
	shaderNoBump->LinkProgram();
	shaders.push_back(shaderNoBump);

	Shader* particleShader = new Shader(SHADERDIR"particleVert.vert", SHADERDIR"particleFrag.frag", SHADERDIR"particleGeom.geom");
	particleShader->LinkProgram();
	shaders.push_back(particleShader);

	Shader* stageShader = new Shader(SHADERDIR"shadowVert.vert", SHADERDIR"shadowFrag.frag");
	stageShader->LinkProgram();
	shaders.push_back(stageShader);



	/* Textures */
	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG", "tex");
	tex->ToggleRepeating();
	textures.push_back(tex);

	Texture* bricks = new Texture(TEXTUREDIR "brick.tga", "tex");
	bricks->ToggleRepeating();
	textures.push_back(bricks);

	Texture* brickBump = new Texture(TEXTUREDIR "brickDOT3.tga", "bumpTex");
	brickBump->ToggleRepeating();
	textures.push_back(brickBump);

	Texture* bumpTex = new Texture(TEXTUREDIR"Barren RedsDOT3.JPG", "bumpTex");
	bumpTex->ToggleRepeating();
	textures.push_back(bumpTex);

	//Source: https://poly.google.com/view/0vzzmM-t8CP
	Texture* fireTex = new Texture(TEXTUREDIR"fire.png", "tex");
	textures.push_back(fireTex);


	Texture* emberTex = new Texture(TEXTUREDIR"firetrail.tga", "diffuseTex");
	textures.push_back(emberTex);

	Texture* smokeTex = new Texture(TEXTUREDIR"smoke.png", "diffuseTex");
	textures.push_back(smokeTex);

	Texture* hellTex = new Texture(TEXTUREDIR"hellknight.tga", "tex");
	textures.push_back(hellTex);

	Texture* hellBumpTex = new Texture(TEXTUREDIR"hellknight_local.tga", "bumpTex");
	textures.push_back(hellBumpTex);


	/* Meshes */
	OBJMesh* fire = new OBJMesh(MESHDIR"fire-fix.obj");
	meshes.push_back(fire);

	Mesh* quadMesh = Mesh::GenerateQuad();
	meshes.push_back(quadMesh);

	ParticleEmitter* smoke = new ParticleEmitter();
	smoke->SetParticleSize(40.0f);
	smoke->SetParticleVariance(1.0f);
	smoke->SetLaunchParticles(5.0f);
	smoke->SetParticleLifetime(10000.0f);
	smoke->SetParticleSpeed(0.1f);
	smoke->SetSpawnWidth(80.0f);
	meshes.push_back(smoke);

	ParticleEmitter* embers = new ParticleEmitter();
	embers->SetParticleSize(15.0f);
	embers->SetParticleVariance(1.0f);
	embers->SetLaunchParticles(2.0f);
	embers->SetParticleLifetime(1500.0f);
	embers->SetParticleSpeed(0.5f);
	embers->SetSpawnWidth(80.0f);
	meshes.push_back(embers);

	
	/* Scene Nodes */
	/* Knights */
	MD5FileData*	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	MD5Node*		hellNode = new MD5Node(*hellData);
	hellNode->SetShader(hellKnightShader);
	hellNode->SetBoundingRadius(1000.0f);
	hellNode->SetTransform(Matrix4::Translation(Vector3(0, 0, -700)));
	hellNode->SetRotation(Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
	hellNode->SetScale(Vector3(4, 4, 4));

	hellData->AddAnim(MESHDIR"idle2.md5anim");
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");

	hellNode->AddTexture(hellTex);
	hellNode->AddTexture(hellBumpTex);

	MD5Node*		hellNode2 = new MD5Node(*hellData);
	hellNode2->SetShader(hellKnightShader);
	hellNode2->SetBoundingRadius(1000.0f);
	hellNode2->SetTransform(Matrix4::Translation(Vector3(0, 0, 700)));
	hellNode2->SetRotation(Matrix4::Rotation(-90.0f, Vector3(0, 1, 0)));
	hellNode2->SetScale(Vector3(4, 4, 4));

	hellNode2->PlayAnim(MESHDIR"idle2.md5anim");

	hellNode2->AddTexture(hellTex);
	hellNode2->AddTexture(hellBumpTex);


	/* Room */
	SceneNode* roomNode = new SceneNode();

	SceneNode* floor = new SceneNode(shader, quadMesh);
	floor->AddTexture(tex);
	floor->AddTexture(bumpTex);
	floor->SetScale(Vector3(1000, 1000, 1000));
	floor->SetRotation(Matrix4::Rotation(90.0f, Vector3(1,0,0)));
	floor->SetBoundingRadius(1300.0f);
	roomNode->AddChild(floor);

	SceneNode* wall1 = new SceneNode(shader, quadMesh);
	wall1->AddTexture(bricks);
	wall1->AddTexture(brickBump);
	wall1->SetTransform(Matrix4::Translation(Vector3(0, 1000, 1000)));
	wall1->SetScale(Vector3(1000, 1000, 1000));
	wall1->SetRotation(Matrix4::Rotation(0.0f, Vector3(0, 1, 0)));
	wall1->SetBoundingRadius(1300.0f);
	roomNode->AddChild(wall1);

	SceneNode* wall2 = new SceneNode(shader, quadMesh);
	wall2->AddTexture(bricks);
	wall2->AddTexture(brickBump);
	wall2->SetTransform(Matrix4::Translation(Vector3(1000, 1000, 0)));
	wall2->SetScale(Vector3(1000, 1000, 1000));
	wall2->SetRotation(Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
	wall2->SetBoundingRadius(1300.0f);
	roomNode->AddChild(wall2);

	SceneNode* wall3 = new SceneNode(shader, quadMesh);
	wall3->AddTexture(bricks);
	wall3->AddTexture(brickBump);
	wall3->SetTransform(Matrix4::Translation(Vector3(-1000, 0, 0)));
	wall3->SetScale(Vector3(1000, 1000, 1000));
	wall3->SetRotation(Matrix4::Rotation(-90.0f, Vector3(0, 1, 0)));
	wall3->SetBoundingRadius(1300.0f);
	roomNode->AddChild(wall3);

	SceneNode* wall4 = new SceneNode(shader, quadMesh);
	wall4->AddTexture(bricks);
	wall4->AddTexture(brickBump);
	wall4->SetTransform(Matrix4::Translation(Vector3(0, 1000, -1000)));
	wall4->SetScale(Vector3(1000, 1000, 1000));
	wall4->SetRotation(Matrix4::Rotation(-180.0f, Vector3(0, 1, 0)));
	wall4->SetBoundingRadius(1300.0f);
	roomNode->AddChild(wall4);

	SceneNode* roof = new SceneNode(shader, quadMesh);
	roof->AddTexture(tex);
	roof->AddTexture(bumpTex);
	roof->SetTransform(Matrix4::Translation(Vector3(0, 1000, 0)));
	roof->SetScale(Vector3(1000, 1000, 1000));
	roof->SetRotation(Matrix4::Rotation(-90.0f, Vector3(1, 0, 0)));
	roof->SetBoundingRadius(1300.0f);
	roomNode->AddChild(roof);


	/* Fire */
	SceneNode* firePlace = new SceneNode(basicShader, fire);
	firePlace->SetTransform(Matrix4::Translation(Vector3(0, 40, 0)));
	firePlace->SetScale(Vector3(80, 80, 80));
	firePlace->AddTexture(fireTex);
	firePlace->SetBoundingRadius(1000.0f);

	ParticleNode* smokeEmitter = new ParticleNode(particleShader, smoke);
	smokeEmitter->AddTexture(smokeTex);
	smokeEmitter->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	smokeEmitter->SetBoundingRadius(1000.0f);
	smokeEmitter->SetColour(Vector4(1.0, 1.0, 1.0, 0.5));

	ParticleNode* emberEmitter = new ParticleNode(particleShader, embers);
	emberEmitter->AddTexture(emberTex);
	emberEmitter->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	emberEmitter->SetBoundingRadius(200.0f);
	emberEmitter->SetColour(Vector4(1.0, 1.0, 1.0, 0.5));

	firePlace->AddChild(smokeEmitter);
	firePlace->AddChild(emberEmitter);

	/* Lights */
	LightNode* light1 = new LightNode(stageShader, nullptr, Vector4(1.0, 0.67, 0.0, 1.0), 5000.0f, Vector3(0, 10, 0));
	light1->SetDirection(Vector3(0, 300, -1000));
	scene->AddLight(light1);

	LightNode* light2 = new LightNode(stageShader, nullptr, Vector4(1.0, 0.67, 0.0, 1.0), 5000.0f, Vector3(0, 10, 0));
	light2->SetDirection(Vector3(0, 300, 1000));
	scene->AddLight(light2);

	LightNode* light3 = new LightNode(stageShader, nullptr, Vector4(1.0, 0.67, 0.0, 1.0), 1000.0f, Vector3(0, 0, 0));
	light3->SetDirection(Vector3(0, 1000, 0));
	scene->AddLight(light3);

	/* Adding nodes to scene */
	scene->AddNonShadowCastingNode(smokeEmitter);
	scene->AddNonShadowCastingNode(emberEmitter);
	scene->AddNonShadowCastingNode(firePlace);


	scene->AddNode(hellNode);
	scene->AddNode(hellNode2);
	scene->AddNode(firePlace);
	scene->AddNode(roomNode);



	vector<RenderStages> stages = { SHADOW_STAGE};
	scene->SetRenderStages(stages);

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	scene->GetCamera()->SetPosition(Vector3(900,10,0));
	scene->GetCamera()->SetPitch(15);
	scene->GetCamera()->SetYaw(0);

	scenes[1] = scene;

	return scene;
}


Scene* SceneLoader::LoadScene3() {
	Scene* scene = new Scene();

	/* Shaders */
	Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"bufferNoBumpFrag.frag");
	shader->LinkProgram();
	shaders.push_back(shader);

	Shader* pointLightShader = new Shader(SHADERDIR"pointLightVert.vert", SHADERDIR"pointLightFrag.frag");
	pointLightShader->LinkProgram();
	shaders.push_back(pointLightShader);

	Shader* skyBoxShader = new Shader(SHADERDIR"skyboxVert.vert", SHADERDIR"skyboxFrag.frag");
	skyBoxShader->LinkProgram();
	shaders.push_back(skyBoxShader);

	/* Textures */
	Texture* tex = new Texture(TEXTUREDIR "brush.png", "tex");
	tex->ToggleRepeating();
	textures.push_back(tex);


	Texture* colourTex = new Texture("depthTex");
	textures.push_back(colourTex);

	Texture* normalTex = new Texture("normTex");
	textures.push_back(normalTex);

	string skyTextures[6] = { TEXTUREDIR "desert_night_rt.tga",TEXTUREDIR "desert_night_lf.tga",TEXTUREDIR "desert_night_up.tga",TEXTUREDIR "desert_night_dn.tga",TEXTUREDIR "desert_night_bk.tga",TEXTUREDIR "desert_night_ft.tga" };
	CubeMapTexture* skyCubeMap = new CubeMapTexture(skyTextures, "cubeTex");
	textures.push_back(skyCubeMap);

	/* Meshes */
	OBJMesh* sphere = new OBJMesh();
	sphere->LoadOBJMesh(MESHDIR"sphere.obj");
	meshes.push_back(sphere);

	//Source: https://www.turbosquid.com/3d-models/3d-ufo-1210172
	OBJMesh* spaceship = new OBJMesh(MESHDIR"ufo-fix.obj");
	meshes.push_back(spaceship);

	Mesh* quadMesh = Mesh::GenerateQuad();
	meshes.push_back(quadMesh);

	SceneNode* spaceshipNode = new SceneNode(shader, spaceship);
	spaceshipNode->AddTexture(tex);
	spaceshipNode->SetBoundingRadius(1000000.0f);
	spaceshipNode->SetTransform(Matrix4::Translation(Vector3(0,0,0)));
	spaceshipNode->SetScale(Vector3(100, 100, 100));


	SceneNode* skybox = new SceneNode(skyBoxShader, quadMesh);
	skybox->SetDepthTest(false);
	skybox->AddTexture(skyCubeMap);
	skybox->SetBoundingRadius(1000000000.0f);
	skybox->SetFaceCulling(false);

	


	
	/* Lights */
	int stars = 10;
	float angle = 0;
	float step = (2 * PI) / stars;
	float radius = 80;
	float asteroidWidth = 1000;

	for (int i = 0; i < stars; ++i) {
		float x = (asteroidWidth + radius) * cos(angle);
		float z = (asteroidWidth + radius) * sin(angle);
		angle += step;
		Vector3 pos = Vector3(x, 400, z);

		Vector4 colour = Vector4(0.5f + (float)(rand() % 129) / 128.0f,
			0.5f + (float)(rand() % 129) / 128.0f,
			0.5f + (float)(rand() % 129) / 128.0f,
			1.0f);

		float radius = (600);
		LightNode* temp = new LightNode(pointLightShader, sphere, colour, radius, pos);
		temp->AddTexture(colourTex);
		temp->AddTexture(normalTex);
		scene->AddLight(temp);

	}

	angle = 0;
	for (int i = 0; i < stars; ++i) {
		float x = (asteroidWidth + radius) * cos(angle);
		float z = (asteroidWidth + radius) * sin(angle);
		angle += step;
		Vector3 pos = Vector3(x, -400, z);

		Vector4 colour = Vector4(0.5f + (float)(rand() % 129) / 128.0f,
			0.5f + (float)(rand() % 129) / 128.0f,
			0.5f + (float)(rand() % 129) / 128.0f,
			1.0f);

		float radius = (600);
		LightNode* temp = new LightNode(pointLightShader, sphere, colour, radius, pos);
		temp->AddTexture(colourTex);
		temp->AddTexture(normalTex);
		scene->AddLight(temp);

	}

	LightNode* temp = new LightNode(pointLightShader, sphere, Vector4(0.796,0.945,1.0,1.0), 4000, Vector3(0,2000, 3250));
	temp->AddTexture(colourTex);
	temp->AddTexture(normalTex);
	scene->AddLight(temp);

	/* Adding nodes to scene */
	scene->SetSkyBox(skybox);
	scene->AddNode(spaceshipNode);

	/* Initial Build */
	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	/* Set stages required */
	vector<RenderStages> stages = { DEFERRED_LIGHT_STAGE };
	scene->SetRenderStages(stages);

	/* Position Camera */
	scene->GetCamera()->SetPosition(Vector3(0,1200,-2000));
	scene->GetCamera()->SetPitch(-15);
	scene->GetCamera()->SetYaw(180);
	scenes[2] = scene;
	return scene;
}
