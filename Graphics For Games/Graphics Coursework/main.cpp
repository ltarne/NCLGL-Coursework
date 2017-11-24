#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "../../nclgl/RenderingManager.h"
#include "SceneLoader.h"
#include <time.h>

int main() {
	int width = 1920;
	int height = 1080;
	Window w("Graphics Coursework", width, height, true);
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
	
	w.GetTimer()->GetTimedMS();
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float msec = w.GetTimer()->GetTimedMS();

		//FPS Timer
		time += msec;
		sceneTimer += msec;
		frameCount += 1;
		if ((time / 1000.0f) > 1) {
			fps = round(frameCount);
			frameCount = 0;
			time = 0.0f;
		}

		//Auto Transition
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

		//Post Processing Settings
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {

			manager.TogglePostProcessingEffect(BLOOM_STAGE);
		}
		else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {

			manager.TogglePostProcessingEffect(COLOUR_CORRECTION_STAGE);
		}

		//Manual Scene Transition
		else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT)) {

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
			pause = !pause;
			manager.TogglePause();
		}

		if (!pause) {
			scenes[currentScene]->GetCamera()->SetYaw(abs(sin(sceneTimer*0.00007)) * 300);
		}
		

		//Update and Draw
		manager.UpdateScene(msec);
		manager.DrawScene();
	}

	delete scene1;
	delete scene2;
	delete scene3;

	return 0;
}