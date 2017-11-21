#pragma once
#include "SceneNode.h"
#include "ParticleEmitter.h"
class ParticleNode :
	public SceneNode
{
public:
	ParticleNode(Shader* shader = nullptr, ParticleEmitter* emitter = nullptr, Vector4 colour = Vector4(1, 1, 1, 1));
	~ParticleNode();

	virtual void LoadUniforms(Shader* shader);

	virtual void Update(float msec);
	virtual void Draw(const OGLRenderer &renderer, Shader* overrideShader = nullptr);

protected:
	ParticleEmitter* emitter;

};

