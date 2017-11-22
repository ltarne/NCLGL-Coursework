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

	scene1->SetProjMatrix(Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f));
	scene2->SetProjMatrix(Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f));
	scene3->SetProjMatrix(Matrix4::Perspective(1.0f, 100000.0f, (float)width / (float)height, 45.0f));

	RenderingManager manager(&renderer);

	Scene* scenes[3] = { scene1, scene2, scene3 };

	manager.SetActiveScene(scene1);

	int currentScene = 0;

	int frameCount = 0;
	float time = 0.0f;

	float sceneTimer = 0;
	bool pause = false;
	

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float msec = w.GetTimer()->GetTimedMS();
		time += msec;
		sceneTimer += msec;
		frameCount += 1;
		if ((time / 1000.0f) > 1) {
			fps = round(frameCount);
			frameCount = 0;
			time = 0.0f;
		}

		if ((sceneTimer / 60000) > 1 && !pause) {
			if (currentScene == 2) {
				currentScene = 0;
			}
			else {
				currentScene++;
			}
			manager.SetActiveScene(scenes[currentScene]);
			sceneTimer = 0;
		}


		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
			manager.SetActiveScene(scene1);
			currentScene = 0;
			sceneTimer = 0;
		}
		else if (Window::GetKeyboard()->KeyDown(KEYBOARD_2)) {
			manager.SetActiveScene(scene2);
			currentScene = 1;
			sceneTimer = 0;
		}
		else if (Window::GetKeyboard()->KeyDown(KEYBOARD_3)) {
			manager.SetActiveScene(scene3);
			currentScene = 2;
			sceneTimer = 0;
		}
		else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT)) {
			//manager.SetActiveScene(scene3);
			if (currentScene == 0) {
				currentScene = 2;
			}
			else {
				currentScene--;
			}
			manager.SetActiveScene(scenes[currentScene]);
			sceneTimer = 0;
		}
		else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT)) {
			//manager.SetActiveScene(scene3);
			if (currentScene == 2) {
				currentScene = 0;
			}
			else {
				currentScene++;
			}
			manager.SetActiveScene(scenes[currentScene]);
			sceneTimer = 0;
		}
		else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_PAUSE)) {
			//manager.SetActiveScene(scene3);
			pause = !pause;
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