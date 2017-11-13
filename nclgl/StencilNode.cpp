#include "StencilNode.h"




StencilNode::StencilNode(Shader * shader, Mesh * mesh, Vector4 colour) 
	:SceneNode(shader, mesh, colour) {

}

StencilNode::~StencilNode()
{
}

void StencilNode::Draw(const OGLRenderer & renderer) {
	LoadUniforms();
	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 2, ~0);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	if (mesh != nullptr) {
		mesh->Draw();
	}

	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 2, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}


