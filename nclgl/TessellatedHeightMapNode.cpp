#include "TessellatedHeightMapNode.h"


TessellatedHeightMapNode::TessellatedHeightMapNode(int tiles, int scale, Shader * shader, Mesh * mesh, Vector4 colour) {
	int line = sqrt(tiles);
	for (int x = 0; x < line; ++x) {
		for (int z = 0; z < line; ++z) {
			SceneNode* tile = new SceneNode(shader, mesh);
			tile->SetTransform(Matrix4::Translation(Vector3(x*(scale * 2), 0, z * (scale * 2))));
			tile->SetRotation(Matrix4::Rotation(-90, Vector3(1, 0, 0)));
			tile->SetScale(Vector3(scale,scale,scale));
			tile->SetBoundingRadius(scale *2);
			
			AddChild(tile);
		}
	}
}

TessellatedHeightMapNode::~TessellatedHeightMapNode() {
}
