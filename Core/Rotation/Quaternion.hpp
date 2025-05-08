//
//  Quaternion.hpp
//  TinyMath3D
//
//  Created by Mostafa on 03.05.25.
//

#include <Core/Vectors/Vector3.hpp>
#include <stdio.h>

using TinyMath3D::Vectors::Vector3;

namespace TinyMath3D::Rotation {
class Quaternion {
  public:
	float w = 1.0f;
	float x = 0.0f, y = 0.0f, z = 0.0f;

	// Default: identity quaternion
	Quaternion() = default;
	Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

	// Construct from angle-axis (assumes axis is normalized)
	static Quaternion FromAxisAngle(float angle, const Vector3 &axis) {
		float halfAngle = 0.5f * angle;
		float s = std::sin(halfAngle);
		return Quaternion(std::cos(halfAngle), axis.x * s, axis.y * s,
		                  axis.z * s);
	}

	/// @brief Quaternion magnitude (length)
	float Magnitude() const { return std::sqrt(w * w + x * x + y * y + z * z); }

	/// @brief Normalize in-place
	void Normalize() {
		float m = Magnitude();
		if (m > 0.0f) {
			float invMag = 1.0f / m;
			w *= invMag;
			x *= invMag;
			y *= invMag;
			z *= invMag;
		}
	}

	/// @brief Return a normalized copy
	Quaternion Normalized() const {
		Quaternion result = *this;
		result.Normalize();
		return result;
	}

	/// @brief Return the conjugate (negate vector part)
	Quaternion Conjugate() const { return Quaternion(w, -x, -y, -z); }

	/// @brief Return the inverse (conjugate over magnitude squared)
	Quaternion Inverse() const {
		float magSq = w * w + x * x + y * y + z * z;
		if (magSq > 0.0f) {
			float invMagSq = 1.0f / magSq;
			return Quaternion(w * invMagSq, -x * invMagSq, -y * invMagSq,
			                  -z * invMagSq);
		}
		return Quaternion(); // return identity if degenerate
	}

	/// @brief Quaternion multiplication (composition of rotations)
	Quaternion operator*(const Quaternion &q) const {
		return Quaternion(w * q.w - x * q.x - y * q.y - z * q.z,
		                  w * q.x + x * q.w + y * q.z - z * q.y,
		                  w * q.y - x * q.z + y * q.w + z * q.x,
		                  w * q.z + x * q.y - y * q.x + z * q.w);
	}

	/// @brief Rotate a vector by this quaternion
	Vector3 RotateVector(const Vector3 &v) const {
		Quaternion vq(0, v.x, v.y, v.z);
		Quaternion rq = (*this) * vq * this->Inverse();
		return Vector3(rq.x, rq.y, rq.z);
	}

	/// @brief Identity quaternion
	static Quaternion Identity() { return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); }
};
} // namespace TinyMath3D::Rotation
