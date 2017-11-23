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

	/* Shaders */
	/*Shader* shader = new Shader(SHADERDIR"shadowSceneVert.vert", SHADERDIR"shadowSceneFrag.frag");
	shader->LinkProgram();*/

	Shader* shader = new Shader(SHADERDIR"heightMapShadowVert.vert", SHADERDIR"heightMapMultiFrag.frag", "", SHADERDIR"heightMapTCS.tesc", SHADERDIR"heightMapTES.tese");
	if (!shader->LinkProgram()) {
		return NULL;
	}
	shaders.push_back(shader);

	Shader* pointLightShader = new Shader(SHADERDIR"pointLightVert.vert", SHADERDIR"pointLightFrag.frag");
	pointLightShader->LinkProgram();
	shaders.push_back(pointLightShader);

	Shader* skyBoxShader = new Shader(SHADERDIR"skyboxVert.vert", SHADERDIR"skyboxFrag.frag");
	skyBoxShader->LinkProgram();
	shaders.push_back(skyBoxShader);

	Shader* reflectionShader = new Shader(SHADERDIR"heightMapShadowVert.vert", SHADERDIR"reflectFrag.frag", "", SHADERDIR"heightMapTCS.tesc", SHADERDIR"waterTES.tese");
	reflectionShader->LinkProgram();
	shaders.push_back(reflectionShader);

	Shader* particleShader = new Shader(SHADERDIR"particleVert.vert", SHADERDIR"particleFrag.frag", SHADERDIR"particleGeom.geom");
	particleShader->LinkProgram();
	shaders.push_back(particleShader);

	float scale = 1000.0f;

	/* Meshes */
	HeightMap* heightMap = new HeightMap(TEXTUREDIR "terrain.raw");
	meshes.push_back(heightMap);
	Mesh* quad = Mesh::GenerateQuad();
	meshes.push_back(quad);
	Mesh* patch = Mesh::GenerateQuadPatch();
	meshes.push_back(patch);

	ParticleEmitter* rain = new ParticleEmitter();
	rain->SetParticleSize(20.0f);
	rain->SetParticleVariance(1.0f);
	rain->SetLaunchParticles(150.0f);
	rain->SetParticleLifetime(3500.0f);
	rain->SetParticleSpeed(0.7f);
	rain->SetDirection(Vector3(0, -1, 0));
	rain->SetOrigin(Vector3((scale * 2) * 2.0, 2500, (scale * 2) * 2.0));
	rain->SetSpawnWidth((scale*2) * 4);
	rain->SetColourShift(Vector4(0.0, 0.4, 0.5, 1.0));
	meshes.push_back(rain);

	/* Textures */
	string skyTextures[6] = { TEXTUREDIR "rusted_west.JPG",TEXTUREDIR "rusted_east.JPG",TEXTUREDIR "rusted_up.JPG",TEXTUREDIR "rusted_down.JPG",TEXTUREDIR "rusted_south.JPG",TEXTUREDIR "rusted_north.JPG" };
	CubeMapTexture* skyCubeMap = new CubeMapTexture(skyTextures, "cubeTex");
	textures.push_back(skyCubeMap);

	Texture* bumpTex = new Texture(TEXTUREDIR"Barren RedsDOT3.JPG", "bumpTex");
	bumpTex->ToggleRepeating();
	textures.push_back(bumpTex);

	Texture* grass = new Texture(TEXTUREDIR"grass.png", "grassTex");
	grass->ToggleRepeating();
	textures.push_back(grass);

	Texture* waterTex = new Texture(TEXTUREDIR"water.png", "waterTex");
	waterTex->ToggleRepeating();
	textures.push_back(waterTex);

	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG", "tex");
	tex->ToggleRepeating();
	textures.push_back(tex);

	Texture* rainTex = new Texture(TEXTUREDIR"rain.png", "diffuseTex");
	textures.push_back(rainTex);

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


	

	/*SceneNode* water = new SceneNode(reflectionShader, quad);
	water->AddTexture(waterTex);
	water->AddTexture(skyCubeMap);
	water->SetTransform(Matrix4::Translation(Vector3(0, 700, 0)));
	water->SetRotation(Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	water->SetScale(Vector3(scale, scale, scale));*/

	//water->SetBoundingRadius(100000000.0f);

	TessellatedHeightMapNode* heightMapRoot = new TessellatedHeightMapNode(16, 1000, shader, patch);
	heightMapRoot->AddTexture(tex);
	heightMapRoot->AddTexture(bumpTex);
	heightMapRoot->AddTexture(grass);

	TessellatedHeightMapNode* water = new TessellatedHeightMapNode(16, scale, reflectionShader, patch);
	water->SetTransform(Matrix4::Translation(Vector3(0, 700, 0)));
	//water->SetRotation(Matrix4::Rotation(180.0f, Vector3(1, 0, 0)));
	water->AddTexture(waterTex);
	water->AddTexture(skyCubeMap);


	scene->AddEffect(rainEmitter);

	scene->AddNode(rainEmitter);
	scene->SetSkyBox(skybox);
	scene->AddNode(heightMapRoot);
	scene->AddNode(water);

	OBJMesh* sphere = new OBJMesh();
	sphere->LoadOBJMesh(MESHDIR"sphere.obj");

	Texture* colourTex = new Texture("depthTex");
	Texture* normalTex = new Texture("normTex");

	for (int i = 0; i < 10; ++i) {
		Vector3 pos = Vector3(0, 1000.0f, -i * 500);

		Vector4 colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		float radius = (1000000);
		LightNode* temp = new LightNode(pointLightShader, sphere, colour, radius, pos);
		temp->AddTexture(colourTex);
		temp->AddTexture(normalTex);
		scene->AddLight(temp);
	}
	


	vector<RenderStages> stages = { BASIC_STAGE, /*COLOUR_CORRECTION_STAGE,*/ PRESENT_STAGE, TEXT_STAGE };
	scene->SetRenderStages(stages);

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	scene->GetCamera()->SetPosition(Vector3(scale*2, 1500, scale * 2));


	scenes[0];

	return scene;
}


Scene* SceneLoader::LoadScene2() {
	Scene* scene = new Scene();

	//Shader* basicShader = new Shader(SHADERDIR"texturedVertex.glsl", SHADERDIR"texturedFragment.glsl");
	/* Shaders */
	Shader* shader = new Shader(SHADERDIR"shadowSceneVert.vert", SHADERDIR"shadowSceneFrag.frag");
	shader->LinkProgram();
	shaders.push_back(shader);

	Shader* shaderNoBump = new Shader(SHADERDIR"shadowSceneVert.vert", SHADERDIR"shadowSceneFragNoBump.frag");
	shaderNoBump->LinkProgram();
	shaders.push_back(shaderNoBump);

	Shader* particleShader = new Shader(SHADERDIR"particleVert.vert", SHADERDIR"particleFrag.frag", SHADERDIR"particleGeom.geom");
	particleShader->LinkProgram();
	shaders.push_back(particleShader);


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

	Texture* fireTex = new Texture(TEXTUREDIR"fire.png", "tex");
	//bumpTex->ToggleRepeating();
	textures.push_back(fireTex);

	Texture* emberTex = new Texture(TEXTUREDIR"firetrail.tga", "diffuseTex");
	textures.push_back(emberTex);

	Texture* smokeTex = new Texture(TEXTUREDIR"smoke.png", "diffuseTex");
	textures.push_back(smokeTex);


	/*Texture* shadowMap = new Texture("shadowTex");
	textures.push_back(shadowMap);*/

	/* Meshes */
	OBJMesh* room = new OBJMesh(MESHDIR"cube.obj");
	OBJMesh* fire = new OBJMesh(MESHDIR"fire-fix.obj");

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

	Texture* hellTex = new Texture(TEXTUREDIR"hellknight.tga", "tex");
	//hellTex->SetTexture(hellNode->GetMesh());

	Texture* hellBumpTex = new Texture(TEXTUREDIR"hellknight_local.tga", "bumpTex");
	//hellBumpTex->SetTexture(hellNode->GetMesh()->GetBumpMap());


	MD5FileData*	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	MD5Node*		hellNode = new MD5Node(*hellData);
	hellNode->SetShader(shader);
	hellNode->SetBoundingRadius(1000.0f);
	//hellNode->AddTexture(shadowMap);
	hellNode->SetTransform(Matrix4::Translation(Vector3(0, 0, -700)));
	hellNode->SetRotation(Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
	hellNode->SetScale(Vector3(4, 4, 4));

	hellData->AddAnim(MESHDIR"idle2.md5anim");
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");

	hellNode->AddTexture(hellTex);
	hellNode->AddTexture(hellBumpTex);

	MD5Node*		hellNode = new MD5Node(*hellData);
	hellNode->SetShader(shader);
	hellNode->SetBoundingRadius(1000.0f);
	//hellNode->AddTexture(shadowMap);
	hellNode->SetTransform(Matrix4::Translation(Vector3(0, 0, -700)));
	hellNode->SetRotation(Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
	hellNode->SetScale(Vector3(4, 4, 4));
	

	SceneNode* cube = new SceneNode(shaderNoBump, room);
	cube->SetFaceCulling(false);
	cube->AddTexture(bricks);
	cube->AddTexture(brickBump);
	//cube->AddTexture(shadowMap);
	cube->SetTransform(Matrix4::Translation(Vector3(0, 10, -400)));
	cube->SetScale(Vector3(100, 100, 100));
	cube->SetBoundingRadius(180.0f);


	Mesh* quadMesh = Mesh::GenerateQuad();
	SceneNode* roomNode = new SceneNode();

	SceneNode* floor = new SceneNode(shader, quadMesh);
	floor->AddTexture(tex);
	floor->AddTexture(bumpTex);
	//floor->AddTexture(shadowMap);
	floor->SetScale(Vector3(1000, 1000, 1000));
	floor->SetRotation(Matrix4::Rotation(90.0f, Vector3(1,0,0)));
	floor->SetBoundingRadius(1300.0f);
	roomNode->AddChild(floor);

	SceneNode* wall1 = new SceneNode(shader, quadMesh);
	wall1->AddTexture(bricks);
	wall1->AddTexture(brickBump);
	//wall1->AddTexture(shadowMap);
	wall1->SetTransform(Matrix4::Translation(Vector3(0, 1000, 1000)));
	wall1->SetScale(Vector3(1000, 1000, 1000));
	wall1->SetRotation(Matrix4::Rotation(0.0f, Vector3(0, 1, 0)));
	wall1->SetBoundingRadius(1300.0f);
	roomNode->AddChild(wall1);

	SceneNode* wall2 = new SceneNode(shader, quadMesh);
	wall2->AddTexture(bricks);
	wall2->AddTexture(brickBump);
	//wall2->AddTexture(shadowMap);
	wall2->SetTransform(Matrix4::Translation(Vector3(1000, 1000, 0)));
	wall2->SetScale(Vector3(1000, 1000, 1000));
	wall2->SetRotation(Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
	wall2->SetBoundingRadius(1300.0f);
	roomNode->AddChild(wall2);

	SceneNode* wall3 = new SceneNode(shader, quadMesh);
	wall3->AddTexture(bricks);
	wall3->AddTexture(brickBump);
	//wall3->AddTexture(shadowMap);
	wall3->SetTransform(Matrix4::Translation(Vector3(-1000, 0, 0)));
	wall3->SetScale(Vector3(1000, 1000, 1000));
	wall3->SetRotation(Matrix4::Rotation(-90.0f, Vector3(0, 1, 0)));
	wall3->SetBoundingRadius(1300.0f);
	roomNode->AddChild(wall3);

	SceneNode* wall4 = new SceneNode(shader, quadMesh);
	wall4->AddTexture(bricks);
	wall4->AddTexture(brickBump);
	//wall4->AddTexture(shadowMap);
	wall4->SetTransform(Matrix4::Translation(Vector3(0, 1000, -1000)));
	wall4->SetScale(Vector3(1000, 1000, 1000));
	wall4->SetRotation(Matrix4::Rotation(-180.0f, Vector3(0, 1, 0)));
	wall4->SetBoundingRadius(1300.0f);
	roomNode->AddChild(wall4);

	SceneNode* roof = new SceneNode(shader, quadMesh);
	roof->AddTexture(tex);
	roof->AddTexture(bumpTex);
	//roof->AddTexture(shadowMap);
	roof->SetTransform(Matrix4::Translation(Vector3(0, 1000, 0)));
	roof->SetScale(Vector3(1000, 1000, 1000));
	roof->SetRotation(Matrix4::Rotation(-90.0f, Vector3(1, 0, 0)));
	roof->SetBoundingRadius(1300.0f);
	roomNode->AddChild(roof);



	SceneNode* firePlace = new SceneNode(shaderNoBump, fire);
	firePlace->SetTransform(Matrix4::Translation(Vector3(0, 40, 0)));
	firePlace->SetScale(Vector3(80, 80, 80));
	firePlace->AddTexture(fireTex);
	//firePlace->AddTexture(shadowMap);
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

	scene->AddEffect(smokeEmitter);
	scene->AddEffect(emberEmitter);
	scene->AddEffect(firePlace);


	scene->AddNode(hellNode);
	scene->AddNode(firePlace);
	//scene->AddNode(emberEmitter);
	scene->AddNode(roomNode);

	Shader* stageShader = new Shader(SHADERDIR"shadowVert.vert", SHADERDIR"shadowFrag.frag");
	stageShader->LinkProgram();

	LightNode* light1 = new LightNode(stageShader, nullptr, Vector4(1.0, 0.67, 0.0, 1.0), 10000.0f, Vector3(0, 10, 0));
	light1->SetDirection(Vector3(0, 300, -1000));
	scene->AddLight(light1);

	LightNode* light2 = new LightNode(stageShader, nullptr, Vector4(1.0, 0.67, 0.0, 1.0), 1000.0f, Vector3(0, 10, -200));
	light2->SetDirection(Vector3(0, 0, 1000));
	scene->AddLight(light2);

	//LightNode* light3 = new LightNode(stageShader, nullptr, Vector4(1.0, 0.67, 0.0, 1.0), 1000.0f, Vector3(0, 200, 0));
	//light3->SetDirection(Vector3(1000, 0, 0));
	//scene->AddLight(light3);

	//LightNode* light4 = new LightNode(stageShader, nullptr, Vector4(1.0, 0.67, 0.0, 1.0), 1000.0f, Vector3(0, 200, 0));
	//light4->SetDirection(Vector3(-1000, 0, 0));
	//scene->AddLight(light4);

	LightNode* light5 = new LightNode(stageShader, nullptr, Vector4(1.0, 0.67, 0.0, 1.0), 1000.0f, Vector3(0,0, 0));
	light5->SetDirection(Vector3(0, 1000, 0));
	scene->AddLight(light5);


	vector<RenderStages> stages = { SHADOW_STAGE, PRESENT_STAGE, TEXT_STAGE };
	scene->SetRenderStages(stages);

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	scene->GetCamera()->SetPosition(Vector3(0,400,0));

	scenes[1] = scene;

	return scene;
}


Scene* SceneLoader::LoadScene3() {
	Scene* scene = new Scene();

	/* Shader */
	Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"bufferNoBumpFrag.frag");
	//Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"lightFrag.frag");
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

	Texture* bumpTex = new Texture(TEXTUREDIR"rockBump.png", "bumpTex");
	bumpTex->ToggleRepeating();
	textures.push_back(bumpTex);

	

	string skyTextures[6] = { TEXTUREDIR "desert_night_rt.tga",TEXTUREDIR "desert_night_lf.tga",TEXTUREDIR "desert_night_up.tga",TEXTUREDIR "desert_night_dn.tga",TEXTUREDIR "desert_night_bk.tga",TEXTUREDIR "desert_night_ft.tga" };
	CubeMapTexture* skyCubeMap = new CubeMapTexture(skyTextures, "cubeTex");
	textures.push_back(skyCubeMap);

	/* Meshes */
	HeightMap* heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	meshes.push_back(heightMap);

	//OBJMesh* planet = new OBJMesh(MESHDIR"ufo-fix.obj");

	Mesh* quadMesh = Mesh::GenerateQuad();

	//SceneNode* heightMapNode = new SceneNode(shader, planet);
	//heightMapNode->AddTexture(tex);
	//heightMapNode->AddTexture(bumpTex);
	//heightMapNode->SetBoundingRadius(1000000.0f);
	////heightMapNode->SetTransform(Matrix4::Translation(Vector3(-(RAW_WIDTH * HEIGHTMAP_X) / 2, -350, -(RAW_HEIGHT * HEIGHTMAP_Z))));
	//heightMapNode->SetTransform(Matrix4::Translation(Vector3(0,0,0)));
	//heightMapNode->SetScale(Vector3(100, 100, 100));

	SceneNode* ruler = new SceneNode(shader, quadMesh);
	ruler->AddTexture(tex);
	ruler->AddTexture(bumpTex);
	ruler->SetBoundingRadius(1000000.0f);
	ruler->SetRotation(Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
	ruler->SetScale(Vector3(1300, 1300, 1300));


	SceneNode* skybox = new SceneNode(skyBoxShader, quadMesh);
	skybox->SetDepthTest(false);
	skybox->AddTexture(skyCubeMap);
	skybox->SetBoundingRadius(1000000000.0f);
	skybox->SetFaceCulling(false);

	scene->SetSkyBox(skybox);
	//scene->AddNode(heightMapNode);
	//scene->AddNode(ruler);

	
	OBJMesh* sphere = new OBJMesh();
	sphere->LoadOBJMesh(MESHDIR"sphere.obj");

	Texture* colourTex = new Texture("depthTex");
	Texture* normalTex = new Texture("normTex");

	int stars = 10;
	float angle = 0;
	float step = (2 * PI) / stars;
	float radius = 100;
	float asteroidWidth = 1000;

	for (int i = 0; i < stars; ++i) {
		float x = (asteroidWidth + radius) * cos(angle);
		float z = (asteroidWidth + radius) * sin(angle);
		angle += step;
		Vector3 pos = Vector3(x, 600, z);

		Vector4 colour = Vector4(0.5f + (float)(rand() % 129) / 128.0f,
			0.5f + (float)(rand() % 129) / 128.0f,
			0.5f + (float)(rand() % 129) / 128.0f,
			1.0f);

		float radius = (500);
		LightNode* temp = new LightNode(pointLightShader, sphere, colour, radius, pos);
		temp->AddTexture(colourTex);
		temp->AddTexture(normalTex);
		scene->AddLight(temp);

	}

	/*for (int x = 0; x < LIGHTNUM; ++x) {
		for (int z = 0; z < LIGHTNUM; ++z) {
			Vector3 pos = Vector3(((RAW_WIDTH * HEIGHTMAP_X / (LIGHTNUM - 1)) * x) - (RAW_WIDTH * HEIGHTMAP_X) / 2,
				1000.0f,
				-(RAW_HEIGHT * HEIGHTMAP_Z / (LIGHTNUM - 1)) * z);

			Vector4 colour = Vector4(0.5f + (float)(rand() % 129) / 128.0f,
				0.5f + (float)(rand() % 129) / 128.0f,
				0.5f + (float)(rand() % 129) / 128.0f,
				1.0f);

			float radius = (500);
			LightNode* temp = new LightNode(pointLightShader, sphere, colour, radius, pos);
			temp->AddTexture(colourTex);
			temp->AddTexture(normalTex);
			scene->AddLight(temp);
		}
	}*/

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	vector<RenderStages> stages = { DEFERRED_LIGHT_STAGE, /*BLOOM_STAGE,*/ PRESENT_STAGE, TEXT_STAGE };
	scene->SetRenderStages(stages);

	scene->GetCamera()->SetPosition(Vector3(0,1500,0));

	scenes[2] = scene;
	return scene;
}
