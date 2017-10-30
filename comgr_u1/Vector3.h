#pragma once
#include "Vector2.h"


#define F_PI 3.14159265358979323846f
#define F_E 2.71828182845904523536f

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3(float x, float y, float z) : x(x), y(y), z(z) {};
	Vector3(const Vector2& v) : x(v.x), y(v.y), z(0) {}
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
	Vector3(const Vector3&& v) : x(v.x), y(v.y), z(v.z) {}
	~Vector3() {};

	Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator+(const float& v) const {
		return Vector3(x + v, y + v, z + v);
	}

	Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator-(const float& v) const {
		return Vector3(x - v, y - v, z - v);
	}

	Vector3 operator*(const float& s) const {
		return Vector3(x * s, y * s, z * s);
	}

	Vector3 operator/(const float& s) const {
		return Vector3(x / s, y / s, z / s);
	}

	float operator*(const Vector3& v) const {
		return dot(v);
	}

	Vector3 operator/(const Vector3& v) const {
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	Vector3 multiplyElements(const Vector3& s) const {
		return Vector3(x * s.x, y * s.y, z * s.z);
	}

	Vector3 multiplyElements(const float& d) const {
		return Vector3(x * d, y * d, z * d);
	}

	Vector3 divideElements(const float& d) const {
		return Vector3(x / d, y / d, z / d);
	}

	float distance() const {
		return std::sqrt(x*x + y*y + z*z);
	}

	float distanceSquared() const {
		return x*x + y*y + z*z;
	}

	Vector3 norm() const {
		float f = distance();
		return Vector3(x / f, y / f, z / f);
	}

	// Cross product by using **
	Vector3* operator*() {
		return this;
	}

	Vector3 operator* (const Vector3* v) const {
		return cross(*v);
	}

	Vector3 cross(const Vector3& v) const {
		return Vector3(
			y * v.z - v.y * z,
			z * v.x - v.z * x,
			x * v.y - v.x * y
		);
	}

	Vector3 reflect(const Vector3& n) const {
		return *this - (n*(n**this)*2.0f);
	}
	
	float dot(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	Vector3& operator=(const Vector3 other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	bool operator<(const Vector3 other) const {
		return distance() < other.distance();
	}

	bool operator==(const Vector3 other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	Vector3 minVec(const Vector3& other) {
		return Vector3(x < other.x ? x : other.x, y < other.y ? y : other.y, z < other.z ? z : other.z);
	}
	
	Vector3 maxVec(const Vector3& other) {
		return Vector3(x > other.x ? x : other.x, y > other.y ? y : other.y, z > other.z ? z : other.z);
	}
};
