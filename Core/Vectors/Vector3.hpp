//
//  Vector3.hpp
//  TinyMath3D
//
//  Created by Mostafa on 07.03.25.
//

#pragma once
#include <vector>

namespace TinyMath3D {
namespace Core {
namespace Vectors {
class Vector3 {
  public:
	// Data members
	float x, y, z;

	// Constructors
	Vector3();
	Vector3(float x, float y, float z);

	// Basic Operations
	Vector3 &operator=(const Vector3 &other);
	Vector3 operator+(const Vector3 &other) const;
	Vector3 operator-(const Vector3 &other) const;
	Vector3 operator*(const Vector3 &other) const;
	Vector3 operator/(const Vector3 &other) const;
	Vector3 operator-() const;

	// Compound assignment operators
	Vector3 &operator+=(const Vector3 &other);
	Vector3 &operator-=(const Vector3 &other);
	Vector3 &operator*=(float scalar);
	Vector3 &operator/=(float scalar);

	// Vector Operations
	float length() const;
	Vector3 normalized() const;
	void normalize();
	float dot(const Vector3 &other) const;
	Vector3 cross(const Vector3 &other) const;

	// Utility functions
	float distance(Vector3 &other) const;
	float angle(const Vector3 &other) const;

	// Static utility methods
	static Vector3 zero();
	static Vector3 one();
	static Vector3 right();
	static Vector3 forward();
};

// Non-member opertors
Vector3 operator*(float scaler, const Vector3 &vector);

} // namespace Vectors
} // namespace Core
} // namespace TinyMath3D
