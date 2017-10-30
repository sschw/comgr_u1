#pragma once
#include <cmath>

class Vector2
{
public:
	float x;
	float y;

	Vector2(float x, float y) : x(x), y(y) {};
	Vector2(const Vector2& v) : x(v.x), y(v.y) {};
	Vector2(const Vector2&& v) : x(v.x), y(v.y) {};
	~Vector2() {};

	Vector2 operator+(const Vector2& v) const {
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 operator-(const Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 operator*(const float& s) const {
		return Vector2(x * s, y * s);
	}

	Vector2 operator/(const float& s) const {
		return Vector2(x / s, y / s);
	}

	float operator*(const Vector2& v) const {
		return x * v.x + y * v.y;
	}

	Vector2 operator-() const {
		return Vector2(-x, -y);
	}

	float distance() const {
		return std::sqrt(x*x + y*y);
	}

	Vector2 norm() const {
		float f = distance();
		return Vector2(x / f, y / f);
	}

	Vector2& operator=(const Vector2 other) {
		x = other.x;
		y = other.y;
		return *this;
	}
};
