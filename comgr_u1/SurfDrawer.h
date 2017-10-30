#pragma once
// Comgr1.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <vector>
#include <SDL.h>
#include <cmath>
#include <map>
#include <random>

// Parallelism
#include <ppl.h>

#include "Figure.h"
#include "Ray.h"
#include "Light.h"
#include "Color.h"
#include "Scene.h"
#include "Renderer.h"


using namespace std;

void run(SDL_Window* win, Scene currentScene, SceneRenderer renderer);

// old BVH algorithm
// Figure* findNextFigureBoundingBox(Figure* s1, float minimalLength, vector<Figure*>& s);
