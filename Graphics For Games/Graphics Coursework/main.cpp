#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "../../nclgl/RenderingManager.h"
#include "SceneLoader.h"

int main() {
	Window w("Cube Mapping! sky textures courtesy of http://www.hazelwhorley.com", 800, 600, false);
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

	Scene* scene1 = sceneLoader.LoadScene1();

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