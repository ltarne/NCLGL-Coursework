#pragma once
#include "../../nclgl/Scene.h"
#include "../../nclgl/HeightMap.h"
#include "../../nclgl/MD5Node.h"

class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();

	Scene* LoadScene1();

	Scene* LoadScene2();

	Scene* LoadScene3();
	
};

