#include "Texture.h"

const GLenum Texture::textureUnits[] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7 };

Texture::Texture(string name) {
	this->name = name;
	repeating = false;
	filtering = false;
}

Texture::Texture(string filePath, string name) {
	texture = SOIL_load_OGL_texture(filePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	repeating = false;
	filtering = false;
	this->name = name;
	textureMatrix = Matrix4();
}


Texture::~Texture() {

}

void Texture::ToggleRepeating() {
	repeating = !repeating;
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP); //x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP); //y axis

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::ToggleFiltering() {
	filtering = !filtering;
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::RotateMatrix(float angle) {
	Matrix4 pushPos = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	Matrix4 popPos = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	Matrix4 rotation = Matrix4::Rotation(angle, Vector3(0, 0, 1));
	textureMatrix = pushPos * rotation * popPos;
}

void Texture::LoadTexture(GLuint program, int index) {
	glUniform1i(glGetUniformLocation(program, name.c_str()), index);

	glUniformMatrix4fv(glGetUniformLocation(program, ("textureMatrix" + to_string(index)).c_str()), 1, false, (float*)&(textureMatrix));

	glActiveTexture(Texture::textureUnits[index]);
	glBindTexture(GL_TEXTURE_2D, texture);
}
