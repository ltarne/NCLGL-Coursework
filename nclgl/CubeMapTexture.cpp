#include "CubeMapTexture.h"



CubeMapTexture::CubeMapTexture(string* filePaths, string name) : Texture(name) {
	texture = SOIL_load_OGL_cubemap(filePaths[0].c_str(), filePaths[1].c_str(), filePaths[2].c_str(), filePaths[3].c_str(), filePaths[4].c_str(), filePaths[5].c_str(),
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
}


CubeMapTexture::~CubeMapTexture() {
}

void CubeMapTexture::LoadTexture(GLuint program, int index) {
	glUniform1i(glGetUniformLocation(program, name.c_str()), index);

	glUniformMatrix4fv(glGetUniformLocation(program, ("textureMatrix" + to_string(index)).c_str()), 1, false, (float*)&(textureMatrix));

	glActiveTexture(Texture::textureUnits[index]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}
