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

	Shader* shader = new Shader(SHADERDIR"heightMapShadowVert.vert", SHADERDIR"shadowSceneFrag.frag", "", SHADERDIR"heightMapTCS.tesc", SHADERDIR"heightMapTES.tese");
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

	Shader* reflectionShader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"reflectFrag.frag");
	reflectionShader->LinkProgram();
	shaders.push_back(reflectionShader);

	/* Meshes */
	HeightMap* heightMap = new HeightMap(TEXTUREDIR "terrain.raw");
	meshes.push_back(heightMap);
	Mesh* quad = Mesh::GenerateQuad();
	meshes.push_back(quad);
	Mesh* patch = Mesh::GenerateQuadPatch();
	meshes.push_back(patch);

	/* Textures */
	string skyTextures[6] = { TEXTUREDIR "desert_night_rt.tga",TEXTUREDIR "desert_night_lf.tga",TEXTUREDIR "desert_night_up.tga",TEXTUREDIR "desert_night_dn.tga",TEXTUREDIR "desert_night_bk.tga",TEXTUREDIR "desert_night_ft.tga" };
	CubeMapTexture* skyCubeMap = new CubeMapTexture(skyTextures, "cubeTex");
	textures.push_back(skyCubeMap);

	Texture* bumpTex = new Texture(TEXTUREDIR"Barren RedsDOT3.JPG", "bumpTex");
	bumpTex->ToggleRepeating();
	textures.push_back(bumpTex);

	Texture* waterTex = new Texture(TEXTUREDIR"water.png", "waterTex");
	waterTex->ToggleRepeating();
	textures.push_back(waterTex);

	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG", "tex");
	tex->ToggleRepeating();
	textures.push_back(tex);

	/* Scene Nodes */

	SceneNode* skybox = new SceneNode(skyBoxShader, quad);
	skybox->SetDepthTest(false);
	skybox->AddTexture(skyCubeMap);
	skybox->SetBoundingRadius(1000000000.0f);
	skybox->SetFaceCulling(false);

	SceneNode* water = new SceneNode(reflectionShader, quad);
	water->AddTexture(waterTex);
	water->AddTexture(skyCubeMap);
	water->SetRotation(Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	water->SetScale(Vector3(20,20,20));

	water->SetBoundingRadius(100000000.0f);

	float scale = 1000.0f;

	SceneNode* heightMapRoot = new SceneNode();
	heightMapRoot->SetBoundingRadius(1000000.0f);
	heightMapRoot->SetTransform(Matrix4::Translation(Vector3(0, -10, 0)));
	heightMapRoot->SetScale(Vector3(100, 100, 100));
	SceneNode* node1 = new SceneNode(shader, patch, Vector4(1, 1, 1, 1));
	node1->AddTexture(tex);
	node1->AddTexture(bumpTex);
	node1->SetBoundingRadius(300.0f);
	node1->SetTransform(Matrix4::Translation(Vector3(0, 0, 0))); 
	node1->SetRotation(Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	node1->SetScale(Vector3(scale, scale, scale));
	heightMapRoot->AddChild(node1);
	SceneNode* node2 = new SceneNode(shader, patch, Vector4(1, 1, 1, 1));
	node2->AddTexture(tex);
	node2->AddTexture(bumpTex);
	node2->SetBoundingRadius(300.0f);
	node2->SetTransform(Matrix4::Translation(Vector3(scale, 0, 0)));
	node2->SetRotation(Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	node2->SetScale(Vector3(scale, scale, scale));
	heightMapRoot->AddChild(node2);
	SceneNode* node3 = new SceneNode(shader, patch, Vector4(1, 1, 1, 1));
	node3->AddTexture(tex);
	node3->AddTexture(bumpTex);
	node3->SetBoundingRadius(300.0f);
	node3->SetTransform(Matrix4::Translation(Vector3(20, 0, -scale)));
	node3->SetRotation(Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	node3->SetScale(Vector3(scale, scale, scale));
	heightMapRoot->AddChild(node3);
	SceneNode* node4 = new SceneNode(shader, patch, Vector4(1, 1, 1, 1));
	node4->AddTexture(tex);
	node4->AddTexture(bumpTex);
	node4->SetBoundingRadius(300.0f);
	node4->SetTransform(Matrix4::Translation(Vector3(0, 0, -scale)));
	node4->SetRotation(Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	node4->SetScale(Vector3(scale, scale, scale));
	heightMapRoot->AddChild(node4);

	scene->SetSkyBox(skybox);
	scene->AddNode(heightMapRoot);
	scene->AddNode(water);

	OBJMesh* sphere = new OBJMesh();
	sphere->LoadOBJMesh(MESHDIR"sphere.obj");

	Texture* colourTex = new Texture("depthTex");
	Texture* normalTex = new Texture("normTex");

	Vector3 pos = Vector3(0, 1000.0f, 0);

	Vector4 colour = Vector4(0.5f + (float)(rand() % 129) / 128.0f,
		0.5f + (float)(rand() % 129) / 128.0f,
		0.5f + (float)(rand() % 129) / 128.0f,
		1.0f);

	float radius = (1000000);
	LightNode* temp = new LightNode(pointLightShader, sphere, colour, radius, pos);
	temp->AddTexture(colourTex);
	temp->AddTexture(normalTex);
	scene->AddLight(temp);


	vector<RenderStages> stages = { DEFERRED_LIGHT_STAGE, PRESENT_STAGE, TEXT_STAGE };
	scene->SetRenderStages(stages);

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	scenes[0];

	return scene;
}


Scene* SceneLoader::LoadScene2() {
	Scene* scene = new Scene();

	/* Shaders */
	Shader* shader = new Shader(SHADERDIR"shadowSceneVert.vert", SHADERDIR"shadowSceneFrag.frag");
	//Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"lightFrag.frag");
	shader->LinkProgram();
	shaders.push_back(shader);

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


	Texture* shadowMap = new Texture("shadowTex");
	textures.push_back(shadowMap);

	/* Meshes */
	OBJMesh* room = new OBJMesh(MESHDIR"centeredcube.obj");
	OBJMesh* fire = new OBJMesh(MESHDIR"fire-fix.obj");

	ParticleEmitter* embers = new ParticleEmitter();
	embers->SetParticleSize(2.0f);
	embers->SetParticleVariance(1.0f);
	embers->SetLaunchParticles(16.0f);
	embers->SetParticleLifetime(20000.0f);
	embers->SetParticleSpeed(0.1f);
	meshes.push_back(embers);


	/*MD5FileData*	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	MD5Node*		hellNode = new MD5Node(*hellData);
	hellNode->SetShader(shader);
	hellNode->SetBoundingRadius(1000.0f);
	hellNode->AddTexture(shadowMap);

	hellData->AddAnim(MESHDIR"idle2.md5anim");
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");*/

	SceneNode* cube = new SceneNode(shader, room);
	cube->SetFaceCulling(false);
	cube->AddTexture(bricks);
	cube->AddTexture(brickBump);
	cube->AddTexture(shadowMap);
	cube->SetTransform(Matrix4::Translation(Vector3(0, 10, 0)));
	cube->SetScale(Vector3(100, 100, 100));
	cube->SetBoundingRadius(1000000.0f);


	Mesh* quadMesh = Mesh::GenerateQuad();
	SceneNode* quad = new SceneNode(shader, quadMesh);
	quad->AddTexture(tex);
	quad->AddTexture(bumpTex);
	quad->AddTexture(shadowMap);
	quad->SetScale(Vector3(1000, 1000, 1000));
	quad->SetRotation(Matrix4::Rotation(90.0f, Vector3(1,0,0)));
	quad->SetBoundingRadius(1000000.0f);
	meshes.push_back(quadMesh);

	SceneNode* firePlace = new SceneNode(shader, fire);
	firePlace->SetTransform(Matrix4::Translation(Vector3(0, 40, 0)));
	firePlace->SetScale(Vector3(80, 80, 80));
	firePlace->AddTexture(fireTex);
	firePlace->AddTexture(shadowMap);
	firePlace->SetBoundingRadius(100000.0f);

	SceneNode* emberEmitter = new SceneNode(particleShader, embers);
	emberEmitter->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	emberEmitter->SetBoundingRadius(100000.0f);

	scene->AddEffect(emberEmitter);


	//scene->AddNode(cube);
	scene->AddNode(firePlace);
	scene->AddNode(emberEmitter);
	scene->AddNode(quad);

	Shader* stageShader = new Shader(SHADERDIR"shadowVert.vert", SHADERDIR"shadowFrag.frag");
	stageShader->LinkProgram();

	scene->AddLight(new LightNode(stageShader, nullptr,Vector4(1.0, 0.67, 0.0, 1.0), 1000.0f, Vector3(0,160,0)));


	vector<RenderStages> stages = { SHADOW_STAGE, PRESENT_STAGE, TEXT_STAGE };
	scene->SetRenderStages(stages);

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	scenes[1] = scene;

	return scene;
}


Scene* SceneLoader::LoadScene3() {
	Scene* scene = new Scene();

	/* Shader */
	Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"bufferFrag.frag");
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
	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG", "tex");
	tex->ToggleRepeating();
	textures.push_back(tex);

	Texture* bumpTex = new Texture(TEXTUREDIR"Barren RedsDOT3.JPG", "bumpTex");
	bumpTex->ToggleRepeating();
	textures.push_back(bumpTex);

	string skyTextures[6] = { TEXTUREDIR "rusted_west.JPG",TEXTUREDIR "rusted_east.JPG",TEXTUREDIR "rusted_up.JPG",TEXTUREDIR "rusted_down.JPG",TEXTUREDIR "rusted_south.JPG",TEXTUREDIR "rusted_north.JPG" };
	CubeMapTexture* skyCubeMap = new CubeMapTexture(skyTextures, "cubeTex");
	textures.push_back(skyCubeMap);

	/* Meshes */
	HeightMap* heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	meshes.push_back(heightMap);

	Mesh* quadMesh = Mesh::GenerateQuad();

	SceneNode* heightMapNode = new SceneNode(shader, heightMap);
	heightMapNode->AddTexture(tex);
	heightMapNode->AddTexture(bumpTex);
	//quad->SetScale(Vector3(1000, 1000, 1000));
	//quad->SetRotation(Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	heightMapNode->SetBoundingRadius(1000000.0f);
	heightMapNode->SetTransform(Matrix4::Translation(Vector3(-(RAW_WIDTH * HEIGHTMAP_X) / 2, -350, -(RAW_HEIGHT * HEIGHTMAP_Z))));


	SceneNode* skybox = new SceneNode(skyBoxShader, quadMesh);
	skybox->SetDepthTest(false);
	skybox->AddTexture(skyCubeMap);
	skybox->SetBoundingRadius(1000000000.0f);
	skybox->SetFaceCulling(false);

	scene->SetSkyBox(skybox);
	scene->AddNode(heightMapNode);

	
	OBJMesh* sphere = new OBJMesh();
	sphere->LoadOBJMesh(MESHDIR"sphere.obj");

	Texture* colourTex = new Texture("depthTex");
	Texture* normalTex = new Texture("normTex");

	for (int x = 0; x < LIGHTNUM; ++x) {
		for (int z = 0; z < LIGHTNUM; ++z) {
			Vector3 pos = Vector3(((RAW_WIDTH * HEIGHTMAP_X / (LIGHTNUM - 1)) * x) - (RAW_WIDTH * HEIGHTMAP_X) / 2,
				-100.0f,
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
	}

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	vector<RenderStages> stages = { DEFERRED_LIGHT_STAGE, BLOOM_STAGE, PRESENT_STAGE, TEXT_STAGE };
	scene->SetRenderStages(stages);
	scenes[2] = scene;
	return scene;
}
