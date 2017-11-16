#include "SceneLoader.h"



SceneLoader::SceneLoader() {
}


SceneLoader::~SceneLoader() {
}

Scene* SceneLoader::LoadScene1() {
	Scene* scene = new Scene();


	/*Shader* shader = new Shader(SHADERDIR"shadowSceneVert.vert", SHADERDIR"shadowSceneFrag.frag");
	shader->LinkProgram();*/

	Shader* shader = new Shader(SHADERDIR"heightMapShadowVert.vert", SHADERDIR"shadowSceneFrag.frag", "", SHADERDIR"heightMapTCS.tesc", SHADERDIR"heightMapTES.tese");
	if (!shader->LinkProgram()) {
		return NULL;
	}

	Shader* skyBoxShader = new Shader(SHADERDIR"skyboxVert.vert", SHADERDIR"skyboxFrag.frag");
	skyBoxShader->LinkProgram();

	Shader* reflectionShader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"reflectFrag.frag");
	reflectionShader->LinkProgram();

	HeightMap* heightMap = new HeightMap(TEXTUREDIR "terrain.raw");
	Mesh* quad = Mesh::GenerateQuad();
	Mesh* patch = Mesh::GenerateQuadPatch();

	string textures[6] = { TEXTUREDIR "rusted_west.JPG",TEXTUREDIR "rusted_east.JPG",TEXTUREDIR "rusted_up.JPG",TEXTUREDIR "rusted_down.JPG",TEXTUREDIR "rusted_south.JPG",TEXTUREDIR "rusted_north.JPG" };
	CubeMapTexture* skyCubeMap = new CubeMapTexture(textures, "cubeTex");

	Texture* bumpTex = new Texture(TEXTUREDIR"Barren RedsDOT3.JPG", "bumpTex");
	bumpTex->ToggleRepeating();

	Texture* waterTex = new Texture(TEXTUREDIR"water.png", "waterTex");
	waterTex->ToggleRepeating();

	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG", "tex");
	tex->ToggleRepeating();

	SceneNode* skybox = new SceneNode(skyBoxShader, quad);
	skybox->SetDepthTest(false);
	skybox->AddTexture(skyCubeMap);
	skybox->SetBoundingRadius(1000000000.0f);

	SceneNode* water = new SceneNode(reflectionShader, quad);
	water->AddTexture(waterTex);
	water->AddTexture(skyCubeMap);
	water->SetRotation(Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	water->SetScale(Vector3(20,20,20));

	water->SetBoundingRadius(100000000.0f);

	SceneNode* heightMapRoot = new SceneNode();
	heightMapRoot->SetBoundingRadius(1000000.0f);
	heightMapRoot->SetTransform(Matrix4::Translation(Vector3(0, -10, 0)));
	SceneNode* node1 = new SceneNode(shader, patch, Vector4(1, 1, 1, 1));
	node1->AddTexture(tex);
	node1->AddTexture(bumpTex);
	node1->SetBoundingRadius(300.0f);
	node1->SetTransform(Matrix4::Translation(Vector3(0, 0, 0))); 
	node1->SetRotation(Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	node1->SetScale(Vector3(10, 10, 10));
	heightMapRoot->AddChild(node1);
	SceneNode* node2 = new SceneNode(shader, patch, Vector4(1, 1, 1, 1));
	node2->AddTexture(tex);
	node2->AddTexture(bumpTex);
	node2->SetBoundingRadius(300.0f);
	node2->SetTransform(Matrix4::Translation(Vector3(20, 0, 0)));
	node2->SetRotation(Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	node2->SetScale(Vector3(10, 10, 10));
	heightMapRoot->AddChild(node2);
	SceneNode* node3 = new SceneNode(shader, patch, Vector4(1, 1, 1, 1));
	node3->AddTexture(tex);
	node3->AddTexture(bumpTex);
	node3->SetBoundingRadius(300.0f);
	node3->SetTransform(Matrix4::Translation(Vector3(20, 0, -20)));
	node3->SetRotation(Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	node3->SetScale(Vector3(10, 10, 10));
	heightMapRoot->AddChild(node3);
	SceneNode* node4 = new SceneNode(shader, patch, Vector4(1, 1, 1, 1));
	node4->AddTexture(tex);
	node4->AddTexture(bumpTex);
	node4->SetBoundingRadius(300.0f);
	node4->SetTransform(Matrix4::Translation(Vector3(0, 0, -20)));
	node4->SetRotation(Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	node4->SetScale(Vector3(10, 10, 10));
	heightMapRoot->AddChild(node4);

	scene->AddNode(skybox);
	scene->AddNode(heightMapRoot);
	scene->AddNode(water);

	scene->SetLight(new Light(Vector3(0, 30.0f, -400),
		Vector4(1, 1.0, 1.0, 1), 100000.0f));

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	scenes[0];

	return scene;
}

void SceneLoader::UnloadScene1() {
}

Scene* SceneLoader::LoadScene2() {
	Scene* scene = new Scene();

	Shader* shader = new Shader(SHADERDIR"shadowSceneVert.vert", SHADERDIR"shadowSceneFrag.frag");
	//Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"lightFrag.frag");
	shader->LinkProgram();

	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG", "tex");
	tex->ToggleRepeating();

	Texture* bumpTex = new Texture(TEXTUREDIR"Barren RedsDOT3.JPG", "bumpTex");
	bumpTex->ToggleRepeating();

	MD5FileData*	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	MD5Node*		hellNode = new MD5Node(*hellData);
	hellNode->SetShader(shader);
	hellNode->SetBoundingRadius(1000.0f);

	hellData->AddAnim(MESHDIR"idle2.md5anim");
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");


	SceneNode* quad = new SceneNode(shader, Mesh::GenerateQuad());
	quad->AddTexture(tex);
	quad->AddTexture(bumpTex);
	quad->SetScale(Vector3(1000, 1000, 1000));
	quad->SetRotation(Matrix4::Rotation(90.0f, Vector3(1,0,0)));
	quad->SetBoundingRadius(1000000.0f);


	scene->AddNode(hellNode);
	scene->AddNode(quad);

	scene->SetLight(new Light(Vector3(0,200,-300),
		Vector4(1, 1.0, 1.0, 1), 10000.0f));

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	scenes[1] = scene;

	return scene;
}

void SceneLoader::UnloadScene2() {
	
}

Scene* SceneLoader::LoadScene3() {
	Scene* scene = new Scene();

	Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"bufferFrag.frag");
	//Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"lightFrag.frag");
	shader->LinkProgram();

	Texture* tex = new Texture(TEXTUREDIR "Barren Reds.JPG", "tex");
	tex->ToggleRepeating();

	Texture* bumpTex = new Texture(TEXTUREDIR"Barren RedsDOT3.JPG", "bumpTex");
	bumpTex->ToggleRepeating();


	HeightMap* heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	SceneNode* quad = new SceneNode(shader, heightMap);
	quad->AddTexture(tex);
	quad->AddTexture(bumpTex);
	//quad->SetScale(Vector3(1000, 1000, 1000));
	//quad->SetRotation(Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	quad->SetBoundingRadius(1000000.0f);


	scene->AddNode(quad);

	Shader* pointLightShader = new Shader(SHADERDIR"pointLightVert.vert", SHADERDIR"pointLightFrag.frag");
	pointLightShader->LinkProgram();
	OBJMesh* sphere = new OBJMesh();
	sphere->LoadOBJMesh(MESHDIR"sphere.obj");

	for (int x = 0; x < LIGHTNUM; ++x) {
		for (int z = 0; z < LIGHTNUM; ++z) {
			Vector3 pos = Vector3(((RAW_WIDTH * HEIGHTMAP_X / (LIGHTNUM - 1)) * x),
				500.0f,
				(RAW_HEIGHT * HEIGHTMAP_Z / (LIGHTNUM - 1)) * z);

			Vector4 colour = Vector4(0.5f + (float)(rand() % 129) / 128.0f,
				0.5f + (float)(rand() % 129) / 128.0f,
				0.5f + (float)(rand() % 129) / 128.0f,
				1.0f);

			float radius = (RAW_WIDTH * HEIGHTMAP_X / LIGHTNUM);

			scene->AddLight(new LightNode(pointLightShader, sphere, colour, radius));
		}

	}

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	scenes[2] = scene;
	return scene;
}

void SceneLoader::UnloadScene3() {
}
