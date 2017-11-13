#pragma once
#include "Plane.h"
#include "Matrix4.h"
#include "SceneNode.h"
class Matrix4;//Compile the mat4 class first

class Frustum
{
public:
	Frustum() {}
	~Frustum() {}

	void FromMatrix(const Matrix4 &mvp);
	bool InsideFrustrum(SceneNode &node);

protected:
	Plane planes[6];
};

