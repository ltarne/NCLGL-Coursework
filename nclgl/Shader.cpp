#include "Shader.h"


Shader::Shader(string vertex, string fragment, string geometry, string tcs, string tes) {
	program = glCreateProgram();

	//Generate shader objects
	objects[SHADER_VERTEX] = GenerateShader(vertex, GL_VERTEX_SHADER);
	objects[SHADER_FRAGMENT] = GenerateShader(fragment, GL_FRAGMENT_SHADER);
	objects[SHADER_GEOMETRY] = 0;
	objects[SHADER_TCS] = 0;
	objects[SHADER_TES] = 0;

	if (!geometry.empty()) {
		objects[SHADER_GEOMETRY] = GenerateShader(geometry, GL_GEOMETRY_SHADER);

		//Attach shader to the program
		glAttachShader(program, objects[SHADER_GEOMETRY]);
	}

	if (!tcs.empty()) {
		objects[SHADER_TCS] = GenerateShader(tcs, GL_TESS_CONTROL_SHADER);

		//Attach shader to the program
		glAttachShader(program, objects[SHADER_TCS]);
	}

	if (!tes.empty()) {
		objects[SHADER_TES] = GenerateShader(tes, GL_TESS_EVALUATION_SHADER);

		//Attach shader to the program
		glAttachShader(program, objects[SHADER_TES]);
	}



	//Attach shaders to the program
	glAttachShader(program, objects[SHADER_VERTEX]);
	glAttachShader(program, objects[SHADER_FRAGMENT]);
	SetDefaultAttributes();
}

Shader::~Shader() {
	//might need a fix
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
	cout << "********************************************\n";

	glLinkProgram(program);

	GLint code;
	glGetProgramiv(program, GL_LINK_STATUS, &code);

	if (code == GL_FALSE) {
		cout << "Link Failed!" << endl;
		int max = 512;
		char error[512];
		glGetProgramInfoLog(program, max, &max, &error[0]);
		cout << error << endl << endl;
		loadFailed = true;
		return false;
	}
	cout << "Link success!\n";
	return true;
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
	//cout << into << endl << endl;
	cout << "Loaded shader text!" << endl << endl;

	return true;
}

GLuint Shader::GenerateShader(string from, GLenum type) {
	cout << "**************************************************\n";
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
