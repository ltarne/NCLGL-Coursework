#pragma once
#include "../../nclgl/Scene.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/MD5Node.h"
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/ParticleEmitter.h"

#define LIGHTNUM 8

class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();

	Scene* LoadScene1();

	Scene* LoadScene2();

	Scene* LoadScene3();

protected:
	Scene* scenes[3];

	vector<Shader*> shaders;
	vector<Texture*> textures;
	vector<Mesh*> meshes;
	
};

