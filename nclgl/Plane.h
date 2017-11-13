#pragma once
#include "Vector3.h"

class Plane
{
public:
	Plane() {};
	Plane(const Vector3 &normal, float distance, bool normalise = false);
	~Plane() {};

	inline void SetNormal(const Vector3 &normal) { this->normal = normal; }
	inline Vector3 GetNormal() const { return normal; }

	inline void SetDistance(float distance) { this->distance = distance; }
	inline float GetDistance() const { return distance; }

	bool SphereInPlane(const Vector3 &position, float radius) const;


protected:
	Vector3 normal;
	float distance;
};

