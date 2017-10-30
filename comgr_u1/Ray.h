#pragma once

#include "random_var.h"
#include "Vector3.h"

using namespace std;

class Ray {
public:
	Vector3 origin;
	Vector3 ray;

	Ray(Vector3 origin, Vector3 ray) : origin(origin), ray(ray) {}
	// Create standard Eye Ray
	Ray(Vector3& eye, Vector3& lookat, Vector2& pixel, float fov) : origin(eye), ray(0, 0, 0) {
		Vector3 f = (lookat - eye).norm();
		Vector3 r = (Vector3(0, 1, 0).cross(f.norm())).norm();
		Vector3 u = (f.cross(r)).norm();
		ray = (f + r*pixel.x*tan(fov / 2) + u*pixel.y*tan(fov / 2)).norm();
	}
	// Create a Eye Ray around a radius -> Used for DoF
	Ray(Vector3& eye, Vector3& lookat, Vector2& pixel, float fov, float apart) : origin(eye), ray(0, 0, 0) {
		Vector3 f = (lookat - eye).norm();
		Vector3 r = (Vector3(0, 1, 0).cross(f.norm())).norm();
		Vector3 u = (f.cross(r)).norm();
		Vector3 d = (f + r*pixel.x*tan(fov / 2) + u*pixel.y*tan(fov / 2)).norm();

		Vector3 dLookAt = d * (lookat - eye).distance();
		// Random radius and angle
		float radius = u_01dist() * apart;
		float angle = u_01dist() * 2 * F_PI;

		// Random position on circle in x and y
		float xD = sinf(angle)*radius;
		float yD = cosf(angle)*radius;

		// Translation of eye
		Vector3 move = r*xD + u*yD;

		// New eye
		origin = eye + move;
		ray = (eye + dLookAt - origin).norm();
	}
	
};
