//
//  Vector3.cpp
//  TinyMath3D
//
//  Created by Mostafa on 07.03.25.
//

#include "Vector3.hpp"
#include <algorithm>
#include <cmath>

namespace TinyMath3D::Vectors {

// Constructors
Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

// Basic Operations
Vector3 &Vector3::operator=(const Vector3 &other) {
	if (this != &other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}
	return *this;
}
Vector3 Vector3::operator+(const Vector3 &other) const {
	return Vector3(x + other.x, y + other.y, z + other.z);
}
Vector3 Vector3::operator-(const Vector3 &other) const {
	return Vector3(x - other.x, y - other.y, z - other.z);
}
Vector3 Vector3::operator*(const Vector3 &other) const {
	return Vector3(x * other.x, y * other.y, z * other.z);
}
Vector3 Vector3::operator/(const Vector3 &other) const {
	return Vector3(x / other.x, y / other.y, z / other.z);
}
Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }

// Compound operations
Vector3 &Vector3::operator+=(const Vector3 &other) {
	x = x + other.x;
	y = y + other.y;
	z = z + other.z;
	return *this;
}
Vector3 &Vector3::operator-=(const Vector3 &other) {
	x = x - other.x;
	y = y - other.y;
	z = z - other.z;
	return *this;
}
Vector3 &Vector3::operator*=(float scalar) {
	x = x * scalar;
	y = y * scalar;
	z = z * scalar;
	return *this;
}
Vector3 &Vector3::operator/=(float scalar) {
	x = x / scalar;
	y = y / scalar;
	z = z / scalar;
	return *this;
}

// Vector Operations
float Vector3::length() const { return sqrt(x * x + y * y + z * z); }
void Vector3::normalize() {
	float len = length();
	if (len > 0) {
		float invLen = 1.0f / len;
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}
}
Vector3 Vector3::normalized() const {
	Vector3 result(*this);
	result.normalize();
	return result;
}
float Vector3::dot(const Vector3 &other) const {
	return (x * other.x + y * other.y + z * other.z);
}
Vector3 Vector3::cross(const Vector3 &other) const {
	return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
	               x * other.y - y * other.x);
}

// Utility functions
float Vector3::distance(const Vector3 &other) const {
	float dx = x - other.x;
	float dy = y - other.y;
	float dz = z - other.z;
	return sqrt(dx * dx + dy * dy + dz * dz);
}
float Vector3::angle(const Vector3 &other) const {
	float lenProduct = length() * other.length();
	// Avoid division by zero
	if (lenProduct < 1e-6f)
		return 0.0f;

	// The dot product divided by the product of lengths gives cosine of the
	// angle
	float cosAngle = dot(other) / lenProduct;

	// Clamp to avoid numerical errors that could result in invalid acos input
	cosAngle = std::clamp(cosAngle, -1.0f, 1.0f);

	// Return the angle in radians
	return std::acos(cosAngle);
}
// Static utility methods
Vector3 Vector3::zero() { return Vector3(); }
Vector3 Vector3::one() { return Vector3(1, 1, 1); }
Vector3 Vector3::right() { return Vector3(1, 0, 0); }
Vector3 Vector3::up() { return Vector3(0, 1, 0); }
Vector3 Vector3::forward() { return Vector3(0, 0, 1); }

Vectors::Vector3 operator*(float scalar, const Vector3 &vector) {
	return Vector3(scalar * vector.x, scalar * vector.y, scalar * vector.z);
}
} // namespace TinyMath3D::Vectors
