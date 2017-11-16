#include "Light.h"

//Light::Light(Vector3 position, Vector4 colour, float radius, Shader * shader, Mesh * mesh) 
//	: SceneNode(shader, mesh, colour) {
//	this->position = position;
//	this->transform = Matrix4::Translation(position);
//	this->colour = colour;
//	this->radius = radius;
//}
//
//void Light::Draw(const OGLRenderer & renderer) {
//	LoadUniforms();
//	if (!depthTest) {
//		glDisable(GL_DEPTH_TEST);
//	}
//
//
//	if (mesh != nullptr) {
//		mesh->Draw();
//	}
//
//	if (!depthTest) {
//		glEnable(GL_DEPTH_TEST);
//	}
//}
