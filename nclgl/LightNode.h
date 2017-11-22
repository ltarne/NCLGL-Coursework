#pragma once
#include "SceneNode.h"
class LightNode :
	public SceneNode
{
public:
	LightNode(Shader* shader = nullptr, Mesh* mesh = nullptr, Vector4 colour = Vector4(1, 1, 1, 1), float radius = 10.0f, Vector3 pos = Vector3(0,0,0));
	~LightNode();

	inline void UpdateLightTextures(GLuint depthTex, GLuint normTex) {
		this->depthTex = depthTex;
		this->normTex = normTex;
	}

	virtual void LoadUniforms(Shader* shader);

	void UpdateTextures(GLuint depthTex, GLuint normalTex);
	virtual void Draw(const OGLRenderer &renderer, Shader* overrideShader = nullptr);

	void SetPosition(Vector3 pos) { 
		transform = Matrix4::Translation(pos);
		position = pos;
	}

	inline Vector3 GetPosition() const  { return position; }

	inline float GetRadius() const { return radius; }

	inline void SetDirection(Vector3 direction) { this->direction = direction; }
	inline Vector3 GetDirection() { return direction; }

protected:
	Vector3 position;
	float radius;
	Vector3 direction;

	GLuint depthTex;
	GLuint normTex;
};

