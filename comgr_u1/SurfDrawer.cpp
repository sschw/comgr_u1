// Comgr1.cpp : Defines the entry point for the console application.
//
#include "SurfDrawer.h"

using namespace std;

void run(SDL_Window* win, Scene currentScene, SceneRenderer renderer) {

	SDL_Surface* surf = SDL_GetWindowSurface(win);

	Uint32* pixels = (Uint32 *)surf->pixels;

	concurrency::parallel_for(0, surf->h, [&](int h) {
	//for (int h = 0; h < surf->h; h++) {
		int p = h*surf->w;
		for (int w = 0; w < surf->w; w++, p++) {
			Color c = renderer.renderPixel(w, h);

			if (SDL_MUSTLOCK(surf)) {
				SDL_LockSurface(surf);
			}
			pixels[p] = c;
			if (SDL_MUSTLOCK(surf)) {
				SDL_UnlockSurface(surf);
			}
			SDL_UpdateWindowSurface(win);
		}
	//}
	});

}

// Tree isn't balanced so good - New version in scene
/*void createBVH(vector<Figure*>& s) {
while(s.size() != 1) {
Figure* s1 = s[0];
s.erase(s.begin());
Figure* b1 = findNextFigureBoundingBox(s1, INFINITY, s);
s.push_back(b1);
}
}

Figure* findNextFigureBoundingBox(Figure* s1, float minimalLength, vector<Figure*>& s) {
int i = 0;
int closest = -1;
float closestD = minimalLength;
while(s.begin()+i != s.end()) {
Figure* s2 = s[i];
float d = (s2->pos - s1->pos).distanceSquared();
if (d < closestD)
closest = i, closestD = d;
i++;
}
if(closest == -1) // No closer figure
return s1;

// There is a closer figure that we directly remove from our vector
Figure* s2 = s[closest];
s.erase(s.begin() + closest);

// Returns the closer figure or its bounding box
Figure* f = findNextFigureBoundingBox(s2, closestD, s);
return new FigureBoundingSphere(s1, f);
}*/