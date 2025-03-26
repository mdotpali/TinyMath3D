//
//  Vector3.hpp
//  TinyMath3D
//
//  Created by Mostafa on 07.03.25.
//

#include <algorithm>
#include <cmath>
#include <stdio.h>

namespace TinyMath3D::Vectors {

class Vector3 {
  public:
	// Data members
	float x, y, z;

	// Constructors
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	inline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	inline explicit Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}

	// Static utility methods
	// Static utility methods
	Vector3 inline zero() { return Vector3(); }
	Vector3 inline one() { return Vector3(1, 1, 1); }
	Vector3 inline right() { return Vector3(1, 0, 0); }
	Vector3 inline up() { return Vector3(0, 1, 0); }
	Vector3 inline forward() { return Vector3(0, 0, 1); }
	// Element access
	inline float &operator[](int i) {
		// Potentially add an assert(i >= 0 && i < 4) in debug
		return reinterpret_cast<float *>(this)[i];
	}
	inline float operator[](int i) const {
		// Potentially add an assert(i >= 0 && i < 4) in debug
		return reinterpret_cast<const float *>(this)[i];
	}

	// Basic Operations
	inline Vector3 operator+(const Vector3 &other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	inline Vector3 operator-(const Vector3 &other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	inline Vector3 operator*(const Vector3 &other) const {
		return Vector3(x * other.x, y * other.y, z * other.z);
	}
	inline Vector3 operator/(const Vector3 &other) const {
		return Vector3(x / other.x, y / other.y, z / other.z);
	}
	inline Vector3 operator*(float scalar) const {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}
	inline friend Vector3 operator*(float scalar, const Vector3 &other) {
		return other * scalar;
	}
	inline Vector3 operator-() const { return Vector3(-x, -y, -z); }

	// Compound assignment operators
	inline Vector3 &operator+=(const Vector3 &other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	inline Vector3 &operator-=(const Vector3 &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	inline Vector3 &operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	inline Vector3 &operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	// Comparisons
	inline bool operator==(const Vector3 &other) const {
		return (x == other.x && y == other.y && z == other.z);
	}
	inline bool operator!=(const Vector3 &other) const {
		return !(*this == other);
	}

	// Vector Operations
	inline float length() const { return sqrt(x * x + y * y + z * z); }
	inline float dot(const Vector3 &other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	Vector3 cross(const Vector3 &other) const {
		return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
		               x * other.y - y * other.x);
	}
	inline void normalize() {
		float len = length();
		if (len > 0) {
			float invLen = 1.0f / len;
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}
	}
	inline Vector3 normalized() const {
		Vector3 result(*this);
		result.normalize();
		return result;
	}

	// Utility functions
	inline float distance(const Vector3 &other) const {
		return (other - *this).length();
	}
	inline float angle(const Vector3 &other) const {
		float lenProduct = length() * other.length();
		// Avoid division by zero
		if (lenProduct < 1e-6f)
			return 0.0f;

		// The dot product divided by the product of lengths gives cosine of the
		// angle
		float cosAngle = dot(other) / lenProduct;

		// Clamp to avoid numerical errors that could result in invalid acos
		// input
		cosAngle = std::clamp(cosAngle, -1.0f, 1.0f);

		// Return the angle in radians
		return std::acos(cosAngle);
	}
};

} // namespace TinyMath3D::Vectors
