#pragma once
#include "Vector4.h"
#include "Vector3.h"
//#include "SceneNode.h"

class Light /*: public SceneNode*/
{
public:
	Light(Vector3 position, Vector4 colour, float radius/* Shader* shader = nullptr, Mesh* mesh = nullptr*/)/* : SceneNode(shader,mesh, colour)*/ {
		this->position = position;
		/*this->transform = Matrix4::Translation(position);*/
		this->colour = colour;
		this->radius = radius;
	}
	~Light() {}

	/*virtual void Draw(const OGLRenderer &renderer);*/

	inline Vector3	GetPosition() const				{ return position; }
	inline void		SetPosition(Vector3 position)	{ this->position = position; }

	inline Vector4	GetColour() const			{ return colour; }
	inline void		SetColour(Vector4 colour)	{ this->colour = colour; }

	inline float	GetRadius() const		{ return radius; }
	inline void		SetRadius(float radius) { this->radius = radius; }

protected:
	Vector3 position;
	Vector4 colour;
	float	radius;
};

