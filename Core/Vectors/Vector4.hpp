//
//  Vector4.hpp
//  TinyMath3D
//
//  Created by Mostafa on 25.03.25.
//

#include <algorithm>
#include <cmath>
#include <stdio.h>

namespace TinyMath3D::Vectors {

class Vector4 {
  public:
	// Data members
	float x, y, z, w;

	// Constructors
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	inline Vector4(float x, float y, float z, float w)
	    : x(x), y(y), z(z), w(w) {}
	inline explicit Vector4(float scalar)
	    : x(scalar), y(scalar), z(scalar), w(scalar) {}

	// Static utility methods
	Vector4 inline zero() { return Vector4(); }
	Vector4 inline one() { return Vector4(1, 1, 1, 1); }

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
	inline Vector4 operator+(const Vector4 &other) const {
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	inline Vector4 operator-(const Vector4 &other) const {
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	inline Vector4 operator*(const Vector4 &other) const {
		return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
	}
	inline Vector4 operator/(const Vector4 &other) const {
		return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
	}
	inline Vector4 operator*(float scalar) const {
		return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}
	inline friend Vector4 operator*(float scalar, const Vector4 &other) {
		return (other * scalar);
	}
	inline Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }

	// Compound assignment operators
	inline Vector4 &operator+=(const Vector4 &other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}
	inline Vector4 &operator-=(const Vector4 &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}
	inline Vector4 &operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}
	inline Vector4 &operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	// Vector Operations
	inline float length() const { return sqrt(x * x + y * y + z * z + w * w); }
	inline float dot(const Vector4 &other) const {
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}
	inline void normalize() {
		float len = length();
		if (len > 0) {
			float invLen = 1.0f / len;
			x *= invLen;
			y *= invLen;
			z *= invLen;
			w *= invLen;
		}
	}
	inline Vector4 normalized() const {
		Vector4 result(*this);
		result.normalize();
		return result;
	}

	// Comparisons
	inline bool operator==(const Vector4 &other) const {
		return (x == other.x && y == other.y && z == other.z && w == other.w);
	}
	inline bool operator!=(const Vector4 &other) const {
		return !(*this == other);
	}

	// Utility functions
	inline float distance(const Vector4 &other) const {
		return (other - *this).length();
	}
	inline float angle(const Vector4 &other) const {
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
