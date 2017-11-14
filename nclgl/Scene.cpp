#include "Scene.h"



Scene::Scene() {
	root = new SceneNode();
	camera = new Camera();
	//TODO: Allow multiple lights
}


Scene::~Scene() {
	delete camera;
	delete root;
	ClearNodeList();
}

void Scene::Update(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustrum.FromMatrix(projMatrix * viewMatrix);
	//TODO: Sending of proj and view matrix
	root->Update(msec);
}

void Scene::BuildNodeLists(SceneNode* from) {
	if (frameFrustrum.InsideFrustrum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();

		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}
	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void Scene::QuickSortNodeLists() {
	sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);

	sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Scene::InsertionSortNodeLists() {
	//int i, key, j;
	//for (i = 1; i < n; i++) {
	//	key = arr[i];
	//	j = i - 1;

	//	/* Move elements of arr[0..i-1], that are
	//	greater than key, to one position ahead
	//	of their current position */
	//	while (j >= 0 && arr[j] > key) {
	//		arr[j + 1] = arr[j];
	//		j = j - 1;
	//	}
	//	arr[j + 1] = key;
	//}
	vector<SceneNode*>::iterator i = transparentNodeList.begin();
	i++;
	vector<SceneNode*>::iterator j;
	for (i; i != transparentNodeList.end(); ++i) {
		j = i;

		while (j != transparentNodeList.begin() && !SceneNode::CompareByCameraDistance(*i, *j)) {
			j--;
			
		}
	}
}

void Scene::ClearNodeList() {
	transparentNodeList.clear();
	nodeList.clear();
}
