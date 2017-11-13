#pragma once
#include "../nclgl/OGLRenderer.h"

#define TEXTURE_UNIT_MAX 8

class Texture
{
public:
	static const GLenum textureUnits[TEXTURE_UNIT_MAX];

	Texture(string name = "tex");
	Texture(string filePath, string name);
	~Texture();

	inline GLuint*	GetTexture() { return &texture; }
	inline void		SetTexture(GLuint texture) { this->texture = texture; }

	inline int GetNum() { return num; }

	inline Matrix4 GetTextureMatrix() { return textureMatrix; }
	inline void  SetTextureMatrix(Matrix4 textureMatrix) { this->textureMatrix = textureMatrix; }

	inline string GetName() { return name; }
	inline void SetName(string name) { this->name = name; }

	void ToggleRepeating();

	void ToggleFiltering();

	void RotateMatrix(float angle);

	virtual void LoadTexture(GLuint program, int index);

protected:
	GLuint	texture;
	Matrix4 textureMatrix;
	string name;
	int num;

	

	bool repeating;
	bool filtering;
};

