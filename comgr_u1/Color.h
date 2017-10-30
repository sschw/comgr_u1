#pragma once

#include <SDL.h>
#include <algorithm>
#include "Vector3.h"

using namespace std;

class Color {
public:
	Vector3 color;

	Color() : color(0,0,0) {}
	Color(Uint32 color) : color(colorToLinearColorVector(color)) {}
	Color(Uint8 r, Uint8 g, Uint8 b) : color(srgbToLinear(r), srgbToLinear(g), srgbToLinear(b)) {}
	Color(const Vector3& color) : color(color) {}

	operator Vector3() const {
		return color;
	}

	operator Uint32() const {
		return linearColorVectorToColor(color);
	}

	Vector3 colorToLinearColorVector(Uint32 color) const {
		return Vector3(srgbToLinear((color >> 16) & 0xFF), srgbToLinear((color >> 8) & 0xFF), srgbToLinear(color & 0xFF));
	}

	Uint32 linearColorVectorToColor(const Vector3& color) const {
		return (linearToSrgb(color.x) << 16) + (linearToSrgb(color.y) << 8) + linearToSrgb(color.z);
	}

	float srgbToLinear(const Uint8& c) const {
		return powf(c / 255.0f, 1/2.2f);
	}

	Uint8 linearToSrgb(const float& c) const {
		return (Uint8) (min(pow(c, 2.2f), 1.0f) * 255);
	}

	Color& operator=(const Color c) {
		color = c.color;
		return *this;
	}

	Color operator*(float d) const {
		return Color(Vector3(color.x*d, color.y*d, color.z*d));
	}
};

