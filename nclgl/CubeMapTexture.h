#pragma once
#include "Texture.h"
class CubeMapTexture :
	public Texture
{
public:
	CubeMapTexture(string* filePaths, string name = "cubeMap");
	~CubeMapTexture();

	virtual void LoadTexture(GLuint program, int index);
};

