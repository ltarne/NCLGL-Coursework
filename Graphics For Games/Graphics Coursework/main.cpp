#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "../../nclgl/RenderingManager.h"
#include "SceneLoader.h"
#include <time.h>

int main() {
	int width = 800;
	int height = 600;
	Window w("Graphics Coursework", width, height, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	int fps = 0;

	Renderer renderer(w, &fps);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	//srand(time(NULL));

	SceneLoader sceneLoader;

	Scene* scene1 = sceneLoader.LoadScene1();
	Scene* scene2 = sceneLoader.LoadScene2();
	Scene* scene3 = sceneLoader.LoadScene3();

	scene3->SetProjMatrix(Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f));
	RenderingManager manager(&renderer);

	

	

	manager.SetActiveScene(scene3);

	int frameCount = 0;
	float time = 0.0f;
	

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float msec = w.GetTimer()->GetTimedMS();
		time += msec;
		frameCount += 1;
		if ((time / 1000.0f) > 1) {
			fps = round(frameCount);
			frameCount = 0;
			time = 0.0f;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
			manager.SetActiveScene(scene1);
		}
		else if (Window::GetKeyboard()->KeyDown(KEYBOARD_2)) {
			manager.SetActiveScene(scene2);
		}
		else if (Window::GetKeyboard()->KeyDown(KEYBOARD_3)) {
			manager.SetActiveScene(scene3);
		}

		manager.UpdateScene(msec);
		manager.DrawScene();
	}

	delete scene1;
	delete scene2;
	delete scene3;
	//Delete all textures, shaders etc

	return 0;
}