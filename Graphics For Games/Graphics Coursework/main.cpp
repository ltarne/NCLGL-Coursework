#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "../../nclgl/RenderingManager.h"
#include "SceneLoader.h"

int main() {
	int width = 800;
	int height = 600;
	Window w("Graphics Coursework", width, height, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	RenderingManager manager(&renderer);

	SceneLoader sceneLoader;

	Scene* scene1 = sceneLoader.LoadScene2();

	scene1->SetProjMatrix(Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f));

	manager.SetActiveScene(scene1);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float msec = w.GetTimer()->GetTimedMS();
		manager.UpdateScene(msec);
		manager.DrawScene();
	}

	delete scene1;
	//Delete all textures, shaders etc

	return 0;
}