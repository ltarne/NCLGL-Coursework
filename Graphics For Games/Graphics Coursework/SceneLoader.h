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
	void UnloadScene1();

	Scene* LoadScene2();
	void UnloadScene2();

	Scene* LoadScene3();
	void UnloadScene3();

protected:
	Scene* scenes[3];
	
};

