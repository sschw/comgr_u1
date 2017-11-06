#pragma once
#include <fstream>

#include "Vector3.h"
#include "Ray.h"
#include "Color.h"

using namespace std;

class VisibleFigure;

struct HitPoint {
	const VisibleFigure* figure;
	Vector3 n;
	const Ray& ray;
	float rayMultiplier;
	Color hitColor;
};

class MaterialRenderer {
public:
	virtual Color getColor(Vector3 intersect) = 0;
};

class ColorRenderer : public MaterialRenderer {
public:
	Color color;

	ColorRenderer(Color c) : color(c) {}

	Color getColor(Vector3 intersect) {
		return color;
	}
};

class TextureRenderer : public MaterialRenderer {
public:
	SDL_Surface* tx;

	TextureRenderer(SDL_Surface* t) : tx(t) {}

	Color getColor(Vector3 intersect) {
		float u = atan2(intersect.z, intersect.x) / (2.0f * F_PI) + 0.5f;
		float v = 0.5f-(asin(intersect.y) / F_PI);

		// float u = intersect.x / 2 + 0.5;
		// float v = intersect.y / 2 + 0.5;
		int bytes_per_pixel = tx->format->BytesPerPixel;

		if (SDL_MUSTLOCK(tx)) {
			SDL_LockSurface(tx);
		}
		// We could do tiling here
		Uint32 uIndex = (Uint32)(u * (tx->w));
		Uint32 vIndex = (Uint32)(v * (tx->h));
		
		char* pixels = (char*)tx->pixels;

		// Don't know what SDL does - this somehow works and thats fine
		Uint8 r, g, b;
		SDL_GetRGB(pixels[(uIndex + vIndex*tx->pitch) * bytes_per_pixel], tx->format, &r, &g, &b);
		Color c(r, g, b);
		if (SDL_MUSTLOCK(tx)) {
			SDL_UnlockSurface(tx);
		}
		return c;
	}
};

class HDRRenderer : public MaterialRenderer {
public:
	float* hdr;
	Uint32 width;
	Uint32 height;

	HDRRenderer(float* hdr, Uint32 w, Uint32 h) : hdr(hdr), width(w), height(h) {}
	HDRRenderer(string hdrFile, Uint32 w, Uint32 h) : hdr(nullptr), width(w), height(h) {
		hdr = new float[3 * (w + h*w)];
		ifstream is(hdrFile, ios::binary | ios::in);
		is.read(reinterpret_cast<char*>(hdr), sizeof(float) * 3 * w*h);
		is.close();
	}
	~HDRRenderer() {
		delete[] hdr;
	}

	Color getColor(Vector3 intersect) {
		float r = (1.0f / F_PI)*acos(intersect.z) / sqrt(intersect.x*intersect.x + intersect.y*intersect.y);
		float u = intersect.x*r/2+0.5f;
		float v = intersect.y*r/2+0.5f;
		Uint32 uIndex = (Uint32)(u * (width));
		Uint32 vIndex = (Uint32)(v * (height));

		Color c = Vector3(hdr[3 * (uIndex + vIndex*width)], hdr[3 * (uIndex + vIndex*width)+1], hdr[3 * (uIndex + vIndex*width)+2]);
		return c;
	}
};

class Figure {
public:
	Vector3 pos;
	float radius;

	Figure(Vector3 center, float boundingRadius) : pos(center), radius(boundingRadius) {}
	virtual ~Figure() {}

	bool operator<(const Figure& f) const {
		return pos < f.pos;
	}

	bool operator==(const Figure& other) const {
		return pos == other.pos;
	}

	virtual HitPoint intersect(const Ray& r) const = 0;
};

class FigureBoundingSphere : public Figure {
public:
	Figure* f1;
	Figure* f2;
	FigureBoundingSphere(Figure* f1, Figure* f2) : Figure(Vector3(0,0,0), 0), f1(f1), f2(f2) {
		radius = ((f2->pos - f1->pos).distance() + f1->radius + f2->radius) / 2;
		if (radius < f1->radius)
			radius = f1->radius, pos = f1->pos;
		else if (radius < f2->radius)
			radius = f2->radius, pos = f2->pos;
		else
			pos = f1->pos + ((f2->pos - f1->pos).norm() * (radius - f1->radius));
	}

	~FigureBoundingSphere() {}

	HitPoint intersect(const Ray& r) const {
		Vector3 ce = r.origin - pos;
		float ced = ce*r.ray;
		float cel = ce.distance();
		float n1 = ced*ced - (cel*cel - radius*radius);
		if (n1 < 0)
			return HitPoint{ nullptr, Vector3(0,0,0), r, -1, Color(0x0) };
		float n = sqrt(n1);
		float l1 = -ced + n;
		float l2 = -ced - n;
		if (l1 >= 0 || l2 >= 0) {
			HitPoint le = f1->intersect(r);
			HitPoint ri = f2->intersect(r);
			if (le.rayMultiplier >= 0 && (ri.rayMultiplier < 0 || le.rayMultiplier < ri.rayMultiplier))
				return le;
			if (ri.rayMultiplier >= 0 && (le.rayMultiplier < 0 || ri.rayMultiplier < le.rayMultiplier))
				return ri;
		}
		return HitPoint{ nullptr, Vector3(0,0,0), r, -1, Color(0x0) };
	}
};

class VisibleFigure : public Figure {
public:
	// influenced by light
	bool emiting;
	float diffuseInt;
	float specularInt;
	float specularReflection;
	float reflection;
	float emission;
	MaterialRenderer* r;

	VisibleFigure(Vector3 center, float radius, bool emiting, float dI, float sI, float s, float ref, float emission, MaterialRenderer* r) : Figure(center, radius), emiting(emiting), diffuseInt(dI), specularInt(sI), specularReflection(s), reflection(ref), emission(emission), r(r) {}
	~VisibleFigure() {
		delete r;
	}

	Color getColor(const Ray& ray, float rayMultiplier) const {
		Vector3 intersect = (pos - (ray.origin + ray.ray*rayMultiplier)).norm();
		return r->getColor(intersect);
	};
};

class Plane : public VisibleFigure {
public:
	Vector3 n;
	Plane(Vector3 pos, Vector3 n, float dI, float sI, float s, float ref, float e, Uint32 c) : VisibleFigure(pos, INFINITY, true, dI, sI, s, ref, e, new ColorRenderer(c)), n(n) {}

	HitPoint intersect(const Ray& r) const {
		float den = r.ray*n;
		if (den == 0)
			return HitPoint{ nullptr, n, r, -1, 0 };
		float d = ((pos*n) - (r.origin * n)) / den;
		if(d > 0)
			return HitPoint{ this, n, r, d, getColor(r, d) };
		return HitPoint{ nullptr, n, r, -1, 0 };

	}
};

class Sphere : public VisibleFigure {
public:
	float radius;

	Sphere(Vector3 pos, float r, float dI, float sI, float s, float ref, float e, Uint32 c) : VisibleFigure(pos, r, true, dI, sI, s, ref, e, new ColorRenderer(c)), radius(r) {}
	Sphere(Vector3 pos, float r, float dI, float sI, float s, float ref, float e, SDL_Surface* c) : VisibleFigure(pos, r, true, dI, sI, s, ref, e, new TextureRenderer(c)), radius(r) {}
	Sphere(Vector3 pos, float r, float dI, float sI, float s, float ref, float e, string c, Uint32 w, Uint32 h) : VisibleFigure(pos, r, true, dI, sI, s, ref, e, new HDRRenderer(c, w, h)), radius(r) {}
	// Hack for SkySphere - doesn't have influence of light.
	Sphere(Vector3 pos, float r, float e, string c, Uint32 w, Uint32 h) : VisibleFigure(pos, r, false, 0, 0, 0, 0, e, new HDRRenderer(c, w, h)), radius(r) {}
	Sphere(const Sphere& s) : VisibleFigure(s.pos, s.radius, s.emiting, s.diffuseInt, s.specularInt, s.specularReflection, s.reflection, s.emission, s.r), radius(s.radius) {}
	~Sphere() {}

	HitPoint intersect(const Ray& r) const {
		Vector3 ce = r.origin - pos;
		float ced = ce*r.ray;
		float cel = ce.distance();
		float n1 = ced*ced - (cel*cel - radius*radius);
		if (n1 < 0)
			return HitPoint{ this, Vector3(0,0,0), r, -1, Color(0x0) };
		float n = sqrt(n1);
		float l1 = -ced + n;
		float l2 = -ced - n;
		if (l1 >= 0 && (l2 < 0 || l1 < l2))
			return HitPoint{ this, ((r.origin + r.ray*l1) - pos).norm(), r, l1, getColor(r, l1) };
		if (l2 >= 0 && (l1 < 0 || l2 < l1))
			return HitPoint{ this, ((r.origin + r.ray*l2) - pos).norm(), r, l2, getColor(r, l2) };
		return HitPoint{ this, Vector3(0,0,0), r, -1, Color(0x0) };
	}

	Sphere& operator=(const Sphere other) {
		pos = other.pos;
		radius = other.radius;
		specularReflection = other.specularReflection;
		reflection = other.reflection;
		r = other.r;
		return *this;
	}
};