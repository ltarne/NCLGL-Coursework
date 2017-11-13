#include "Shader.h"


Shader::Shader(string vertex, string fragment, string geometry) {
	program = glCreateProgram();

	//Generate shader objects
	objects[SHADER_VERTEX] = GenerateShader(vertex, GL_VERTEX_SHADER);
	objects[SHADER_FRAGMENT] = GenerateShader(fragment, GL_FRAGMENT_SHADER);
	objects[SHADER_GEOMETRY] = 0;

	if (!geometry.empty()) {
		objects[SHADER_GEOMETRY] = GenerateShader(geometry, GL_GEOMETRY_SHADER);

		//Attach shader to the program
		glAttachShader(program, objects[SHADER_GEOMETRY]);
	}

	//Attach shaders to the program
	glAttachShader(program, objects[SHADER_VERTEX]);
	glAttachShader(program, objects[SHADER_FRAGMENT]);
	SetDefaultAttributes();
}

Shader::~Shader() {
	//Detach shaders from the program
	for (int i = 0; i < 3; ++i) {
		glDetachShader(program, objects[i]);
		glDeleteShader(objects[i]);
	}
	//Delete the program
	glDeleteProgram(program);
}

bool Shader::LinkProgram() {
	if (loadFailed) {
		return false;
	}

	glLinkProgram(program);

	GLint code;
	glGetProgramiv(program, GL_LINK_STATUS, &code);
	return code == GL_TRUE ? true : false;
}

void Shader::SetDefaultAttributes() {
	glBindAttribLocation(program, VERTEX_BUFFER, "position");
	glBindAttribLocation(program, COLOUR_BUFFER, "colour");
	glBindAttribLocation(program, NORMAL_BUFFER, "normal");
	glBindAttribLocation(program, TANGENT_BUFFER, "tangent");
	glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");
}

bool Shader::LoadShaderFile(string from, string & into) {
	ifstream file;
	string temp;

	cout << "Loading shader text from " << from << endl << endl;

	file.open(from.c_str());
	if (!file.is_open()) {
		cout << "File does not exist!\n";
		return false;
	}

	while (!file.eof()) {
		getline(file, temp);
		into += temp + "\n";
	}

	file.close();
	cout << into << endl << endl;
	cout << "Loaded shader text!" << endl << endl;

	return true;
}

GLuint Shader::GenerateShader(string from, GLenum type) {
	cout << "Compiling shader ...\n";

	string load;
	if (!LoadShaderFile(from, load)) {
		cout << "Compiling failed...\n";
		loadFailed = true;
		return 0;
	}

	GLuint shader = glCreateShader(type);

	const char *chars = load.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		cout << "Compiling failed...\n";
		char error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		cout << error;
		loadFailed = true;
		return 0;
	}

	cout << "Compiling success!\n\n";
	loadFailed = false;

	return shader;
}
