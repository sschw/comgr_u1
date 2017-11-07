#pragma once

#include <vector>
#include <map>
#include "Figure.h"
#include "Light.h"


class Scene {
private:
	vector<SDL_Surface*> textures;

	struct FigureDistance {
		Figure* p;
		float d;
	};

	void initCornellBox() {
		// Load Texture
		textures.push_back(SDL_LoadBMP("texture.bmp"));

		figures.insert(figures.end(), {
			new Sphere(Vector3(-1001, 0, 0), 1000, 0.8f, 0.8f, 100, 0, 0, 0x00FF0000),
			new Sphere(Vector3(1001, 0, 0), 1000, 0.8f, 0.8f, 100, 0, 0, 0x000000FF),
			new Sphere(Vector3(0, 0, 1001), 1000, 0.8f, 0.8f, 100, 0, 0, 0x00FFFFFF),
			new Sphere(Vector3(0, -1001, 0), 1000, 0.8f, 0.8f, 100, 0, 0, 0x00FFFFFF),
			new Sphere(Vector3(0, 1001, 0), 1000, 0.8f, 0.8f, 100, 0, 0, 0x00FFFFFF),
			new Sphere(Vector3(-0.6f, 0.6f, -0.6f), 0.4f, 0.8f, 0.8f, 10, 0.0f, 0, textures[0]),
			new Sphere(Vector3(0.3f, 0.4f, 0.3f), 0.6f, 0.8f, 0.8f, 10, 0.1f, 0, 0x00FFFF00)
		});
		light.insert(light.end(), {
			new Light(Vector3(0.0f, -0.9f, -0.6f), 0x00FFFFFF),
			new Light(Vector3(0.0f, -0.0f, -4.0f), 0x00FFFFFF),
			new Light(Vector3(0.0f, -0.9f, -0.7f), 0x00555522)
		});
		createBVH();
	}
	void initSkySphere() {
		figures.insert(figures.end(), {
			new Sphere(Vector3(-0.6f, 0.6f, -0.6f), 0.4f, 0.8f, 0.8f, 10, 0.6f, 0, 0x00AAAAAA),
			new Sphere(Vector3(0.3f, 0.4f, 0.3f), 0.6f, 0.8f, 0.8f, 10, 0.1f, 0, 0x00AAAAAA),
			new Sphere(Vector3(-0.6f, 0.6f, -0.6f), 4.0f, 1.0f, "grace_probe.float", 1000, 1000),
		});
		light.insert(light.end(), {
			new Light(Vector3(0.0f, -0.9f, -0.6f), 0x00FFFFFF),
			new Light(Vector3(0.0f, -0.0f, -4.0f), 0x00FFFFFF),
			new Light(Vector3(0.0f, -0.9f, -0.7f), 0x00555522)
		}); 
		createBVH();
		figures.insert(figures.end(), { new Plane(Vector3(0, 1, 0), Vector3(0, 1, 0), 0.8f, 0.8f, 300, 0.3f, 0, 0x00FFFFFF)});
	}
	void init1024Spheres() {
		for (int i = 0; i < 1024; i++)
			figures.push_back(new Sphere(Vector3((i & 7)*0.3f - 1, ((i & 56) >> 3)*0.3f - 1, ((i & 960) >> 6)*0.5f), 0.1f, 0.8f, 0.8f, 10, 0, 0, 0x0044FF44));
		light.insert(light.end(), {
			new Light(Vector3(0.0f, -0.9f, -0.6f), 0x00FFFFFF),
			new Light(Vector3(0.0f, -0.0f, -4.0f), 0x00FFFFFF),
			new Light(Vector3(0.0f, -0.9f, -0.7f), 0x00555522)
		});
		createBVH();
	}
	void initDepthSpheres() {
		figures.insert(figures.end(), {
			new Sphere(Vector3(-1001, 0, 0), 1000, 0.8f, 0.8f, 100, 0, 0, 0x00FF0000),
			new Sphere(Vector3(1001, 0, 0), 1000, 0.8f, 0.8f, 100, 0, 0, 0x000000FF),
			new Sphere(Vector3(0, 0, 1001), 1000, 0.8f, 0.8f, 100, 0, 0, 0x00FFFFFF),
			new Sphere(Vector3(0, -1001, 0), 1000, 0.8f, 0.8f, 100, 0, 0, 0x00FFFFFF),
			new Sphere(Vector3(0, 1001, 0), 1000, 0.8f, 0.8f, 100, 0, 0, 0x00FFFFFF),
			new Sphere(Vector3(-0.6f, 0.6f, -0.6f), 0.4f, 0.8f, 0.8f, 10, 0.0f, 0, 0x0000FF),
			new Sphere(Vector3(0.3f, 0.4f, 0.3f), 0.6f, 0.8f, 0.8f, 10, 0.1f, 0, 0x00FFFF00)
		});
		light.insert(light.end(), {
			new Light(Vector3(0.0f, -0.9f, -0.6f), 0x00FFFFFF),
			new Light(Vector3(0.0f, -0.0f, -4.0f), 0x00FFFFFF),
			new Light(Vector3(0.0f, -0.9f, -0.7f), 0x00555522)
		});
		lookat = Vector3(0, 0, 0.3f);
		createBVH();
	}

	void initCornellboxPT() {

		figures.insert(figures.end(), {
			new Sphere(Vector3(-1001, 0, 0), 1000, 0.8f, 0.8f, 0, 0, 0, 0x00EE4444),
			new Sphere(Vector3(1001, 0, 0), 1000, 0.8f, 0.8f, 0, 0, 0, 0x004444EE),
			new Sphere(Vector3(0, 0, 1001), 1000, 0.8f, 0.8f, 0, 0, 0, 0x00EEEEEE),
			new Sphere(Vector3(0, -1001, 0), 1000, 0.8f, 0.8f, 0, 0, 0, 0x00EEEEEE),
			new Sphere(Vector3(0, 1001, 0), 1000, 0.8f, 0.8f, 0, 0, 0, 0x00EEEEEE),
			new Sphere(Vector3(-0.6f, 0.6f, -0.6f), 0.4f, 0.8f, 0.8f, 10, 0.0f, 0, 0x003333EE),
			new Sphere(Vector3(0.3f, 0.4f, 0.3f), 0.6f, 0.8f, 0.8f, 10, 0.1f, 0, 0x00EEEE33),
			new Sphere(Vector3(0.0f, -1.5f, -0.3f), 0.6f, 0.8f, 0.8f, 10, 0.1f, 1.5f, 0x00FFFFFF)
		});
		createBVH();
	}
public:
	string name;
	vector<Figure*> figures;
	vector<Light*> light;
	
	float fov = 36.0f / 180.0f * F_PI;
	Vector3 eye;
	Vector3 lookat;

	Scene(string sceneName) : eye(0, 0, -4), lookat(0, 0, 6), name(sceneName) {
		if (name == "cornellbox")
			initCornellBox();
		else if (name == "skysphere")
			initSkySphere();
		else if (name == "1024spheres")
			init1024Spheres();
		else if (name == "depthSpheres")
			initDepthSpheres();
		else if (name == "cornellboxPT") {
			initCornellboxPT();
		}
	}

	~Scene() {
		// We do have memory leak but changing to unique_ptr is too much work...
		// Delete operation would make a heap corruption because the ptr's are in vector.
	}

	operator vector<Figure*>() const {
		return figures;
	}

	operator vector<Light*>() const {
		return light;
	}

	// To make it balanced we put them in every iteration together.
	// Makes a more or less balanced tree.
	void createBVH() {
		while (figures.size() > 1) {
			// Every iteration we combine all elements
			vector<Figure*> notCombined = figures;

			// Clear s to make space for the combinations
			figures.clear();

			// As long as there are elements to combine
			while (notCombined.size() > 1) {
				// Calculate Distances between not combined elements
				map<Figure*, FigureDistance> fBuf;
				for (Figure* f1 : notCombined) {
					fBuf[f1] = { nullptr, INFINITY };
					for (Figure* f2 : notCombined) {
						if (f1 != f2) {
							float d = (f2->pos - f1->pos).distanceSquared();
							if (d < fBuf[f1].d) {
								fBuf[f1].p = f2;
								fBuf[f1].d = d;
							}
						}
					}
				}

				// Loop over all and combine the elements that point to each other.
				for (size_t i = notCombined.size() - 1; i < notCombined.size(); i--) {
					Figure* f1 = notCombined[i];
					// Elements point at each other
					if (f1 == fBuf[fBuf[f1].p].p) {
						// Entry wasn't already evaluated
						if (find(notCombined.begin(), notCombined.end(), fBuf[f1].p) != notCombined.end())
							figures.push_back(new FigureBoundingSphere(f1, fBuf[f1].p));
						// Element doesn't has to be combined in this iteration
						notCombined.erase(notCombined.begin() + i);
					}
				}
			}
			if (notCombined.size() == 1)
				figures.push_back(notCombined[0]);
		}
	}
};