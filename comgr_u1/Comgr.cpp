
#include "SurfDrawer.h"
#include <string>

using namespace std;

int main(int argc, char **argv) {
	SDL_Window* win = SDL_CreateWindow("SDL2 COMGR Task", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);

	Scene scenes[] = { 
		Scene("cornellbox"),
		Scene("skysphere"),
		Scene("1024spheres"),
		Scene("cornellboxPT")
	};

	Scene& currentScene = scenes[0];

	SceneRenderer renderer(currentScene, 400, 400, true, 18, false, 30, 1.0f / 22, true, 100, 4, 3);


	bool quit = false;
	thread t([&]() {
		cout << "--------------------------------------" << endl;
		cout << "|------------------------------------|" << endl;
		cout << "||----------------------------------||" << endl;
		cout << "||| Welcome to this COMGR Renderer |||" << endl;
		cout << "||----------------------------------||" << endl;
		cout << "|------------------------------------|" << endl;
		cout << "--------------------------------------" << endl;
		cout << endl;
		cout << "Resize the window to render a bigger scene." << endl;


		int i = 0;
		int j = 0;
		while (!quit) {
			cout << endl;
			cout << "Current settings are:" << endl;
			cout << "- 1: Scene: " << renderer.scene.name << endl;
			cout << "- 2: Antialiasing: " << renderer.antialiasing << " with " << renderer.antialiasingFactor << " Rays" << endl;
			cout << "- 3: Depth of Field: " << renderer.depthOfField << " with " << renderer.depthOfFieldFactor << " Rays and " << renderer.depthOfFieldApart << " Max Distance" << endl;
			cout << "- 4: Path Tracing: " << renderer.usePathTracing << " with " << renderer.noPtRays << " Rays and " << renderer.noPtDepth << " Reflections" << endl;
			cout << endl;
			cout << "- 5: Close Application" << endl;
			cout << endl;
			cout << "F5 on window to render the scene." << endl;
			cout << "Select an option." << endl;

			cin >> i;
			switch (i) {
			case 1:
				cout << "Select a scene to render: " << endl;
				cout << "- 1: cornellbox" << endl;
				cout << "- 2: skysphere" << endl;
				cout << "- 3: 1024spheres" << endl;
				cout << "- 4: cornellboxPT" << endl;
				cin >> j;
				renderer.scene = scenes[j - 1];
				break;
			case 2:
				cout << "Activate Antialiasing? (0: Off | 1: On)" << endl;
				cin >> renderer.antialiasing;
				if (renderer.antialiasing) {
					cout << "Number of Rays?" << endl;
					cin >> renderer.antialiasingFactor;
				}
				break;
			case 3:
				cout << "Activate DoF? (0: Off | 1: On)" << endl;
				cin >> renderer.depthOfField;
				if (renderer.depthOfField) {
					cout << "Number of Rays?" << endl;
					cin >> renderer.depthOfFieldFactor;
					cout << "Max Distance?" << endl;
					cin >> renderer.depthOfFieldApart;
				}
				break;
			case 4:
				cout << "Use path tracing? (0: Off | 1: On)" << endl;
				cin >> renderer.usePathTracing;
				if (renderer.usePathTracing) {
					cout << "Number of Rays?" << endl;
					cin >> renderer.noPtRays;
					cout << "Depth of Tracing?" << endl;
					cin >> renderer.noPtDepth;
				}
				break;
			case 5:
				quit = true;
				break;
			}
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
	});
	SDL_Event evt;

	while (!quit)
	{
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (evt.type == SDL_KEYDOWN) {
				if(evt.key.keysym.sym == SDLK_F5)
					run(win, currentScene, renderer);
			}
			else if (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_RESIZED) {
				SDL_GetWindowSize(win, (int*) &(renderer.width), (int*) &(renderer.height));
			}
		}

	}
	SDL_DestroyWindow(win);
	SDL_Quit();
	t.join();
	return 0;
}
