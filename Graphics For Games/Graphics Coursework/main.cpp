#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "../../nclgl/Renderer.h"

int main() {
	Window w("Cube Mapping! sky textures courtesy of http://www.hazelwhorley.com", 800, 600, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	Shader* shader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"lightFrag.frag");
	shader->LinkProgram();

	Shader* skyBoxShader = new Shader(SHADERDIR"skyboxVert.vert", SHADERDIR"skyboxFrag.frag");
	skyBoxShader->LinkProgram();

	Shader* reflectionShader = new Shader(SHADERDIR"lightVert.vert", SHADERDIR"reflectFrag.frag");
	reflectionShader->LinkProgram();

	HeightMap* heightMap = new HeightMap(TEXTUREDIR "terrain.raw");
	Mesh* quad = Mesh::GenerateQuad();

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

	renderer.AttachSceneGraph(skybox);
	renderer.AttachSceneGraph(node);

	float waterRotation = 0.0f;

	float heightX = (RAW_WIDTH*HEIGHTMAP_X) / 2.0f;
	float heightY = (256 * HEIGHTMAP_Y) / 3.0f;
	float heightZ = (RAW_HEIGHT*HEIGHTMAP_Z) / 2.0f;
	water->SetTransform(Matrix4::Translation(Vector3(0, -100, 0)));
	water->SetScale(Vector3(RAW_WIDTH*HEIGHTMAP_X / 2.0f, RAW_WIDTH*HEIGHTMAP_X / 2.0f, 10));
	water->SetRotation(Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f)));

	renderer.AttachSceneGraph(water);


	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float msec = w.GetTimer()->GetTimedMS();
		renderer.UpdateScene(msec);
		renderer.RenderScene();

		waterTex->SetTextureMatrix(Matrix4::Rotation(waterRotation, Vector3(0.0f, 0.0f, 1.0f)) * Matrix4::Scale(Vector3(10, 10, 10)));
		waterTex->RotateMatrix(waterRotation);
		waterRotation += msec / 1000.0f;
	}

	delete heightMap;
	delete shader;
	delete tex;
	delete bumpTex;

	return 0;
}