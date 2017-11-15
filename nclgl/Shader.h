#pragma once
#include "OGLRenderer.h"
#include <map>

//#define SHADER_VERTEX 0
//#define SHADER_FRAGMENT 1
//#define SHADER_GEOMETRY 2

enum ShaderStage {
	SHADER_VERTEX = 0,
	SHADER_FRAGMENT,
	SHADER_GEOMETRY,
	SHADER_TCS,
	SHADER_TES,
	SHADER_MAX
};

using namespace std;

class Shader
{
public:
	Shader(string vertex, string fragment, string geometry = "", string tcs = "", string tex = "");
	~Shader();

	GLuint	GetProgram() { return program; }
	bool	LinkProgram();


protected:
	void SetDefaultAttributes();
	bool LoadShaderFile(string from, string &into);
	GLuint GenerateShader(string from, GLenum type);

	GLuint objects[SHADER_MAX];
	GLuint program;

	bool loadFailed;
};

