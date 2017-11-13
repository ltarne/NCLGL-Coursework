#pragma once
#include "Vector4.h"
#include "Vector3.h"

class Light
{
public:
	Light(Vector3 position, Vector4 colour, float radius) {
		this->position = position;
		this->colour = colour;
		this->radius = radius;
	}
	~Light() {}

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

