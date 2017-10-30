#pragma once

#include <algorithm>
#include "Vector3.h"
#include "SDL.h"
#include "Color.h"

using namespace std;
class Light {

public:
	Vector3 pos;
	float intensityAtt = 0.4f;
	Vector3 color;

	Light(Vector3 pos, Color color) : pos(pos), color(color) {}
	Vector3 diffuseLightIntensity(float intensityD, Vector3 n, Vector3 l) const {
		return color.multiplyElements(max(intensityD*(n*l), 0.0f));
	}

	Vector3 spectularLightIntensity(float intensityS, float k, Vector3 n, Vector3 l, Vector3 eye) const {
		Vector3 r = ((n*(l*n)-l) * 2).norm();
		return color.multiplyElements(intensityS*pow(max(r*eye, 0.0f), k));
	}

	Vector3 attenuation(const Vector3& i, const Vector3& hitPoint) const {
		float dist = (pos - hitPoint).distance();
		return i / (1 + intensityAtt*dist*dist);
	}
};
