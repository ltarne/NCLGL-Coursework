#include "SceneLoader.h"



SceneLoader::SceneLoader() {
}


SceneLoader::~SceneLoader() {
}

Scene* SceneLoader::LoadScene1() {
	Scene* scene = new Scene();


	Shader* shader = new Shader(SHADERDIR"shadowSceneVert.vert", SHADERDIR"shadowSceneFrag.frag");
	shader->LinkProgram();

	/*Shader* shader = new Shader(SHADERDIR"shadowSceneVert.vert", SHADERDIR"shadowSceneFrag.frag", "", SHADERDIR"heightMapTCS.tesc", SHADERDIR"heightMapTES.tese");
	shader->LinkProgram();*/

	Shader* skyBoxShader = new Shader(SHADERDIR"skyboxVert.vert", SHADERDIR"skyboxFrag.frag");
	skyBoxShader->LinkProgram();

	Shader* reflectionShader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"reflectFrag.frag");
	reflectionShader->LinkProgram();

	HeightMap* heightMap = new HeightMap(TEXTUREDIR "terrain.raw");
	Mesh* quad = Mesh::GenerateQuad();
	//Mesh* patch = Mesh::GenerateQuadPatch();

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

	water->SetBoundingRadius(100000000.0f);

	SceneNode* node = new SceneNode(shader, heightMap, Vector4(1, 1, 1, 1));
	node->AddTexture(tex);
	node->AddTexture(bumpTex);
	node->SetBoundingRadius(10000.0f);
	node->SetTransform(Matrix4::Translation(Vector3(-((257 * 16.0f) / 2), -300, -((257 * 16.0f) / 2))));

	scene->AttachNode(skybox);
	scene->AttachNode(node);
	scene->AttachNode(water);

	scene->SetLight(new Light(Vector3((RAW_WIDTH*HEIGHTMAP_X) / 2, 500.0f, -(RAW_HEIGHT*HEIGHTMAP_Z) / 2),
		Vector4(1, 1.0, 1.0, 1), (RAW_WIDTH*HEIGHTMAP_X)*2.0f));

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


	scene->AttachNode(hellNode);
	scene->AttachNode(quad);

	scene->SetLight(new Light(Vector3(0,100,-500),
		Vector4(1, 1.0, 1.0, 1), 1000.0f));

	scene->BuildNodeLists(scene->GetRoot());
	scene->QuickSortNodeLists();

	scenes[1] = scene;

	return scene;
}

void SceneLoader::UnloadScene2() {
	
}

Scene* SceneLoader::LoadScene3() {
	Scene* scene = new Scene();

	scenes[2] = scene;
	return scene;
}

void SceneLoader::UnloadScene3() {
}
