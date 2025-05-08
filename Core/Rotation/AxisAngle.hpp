//
//  AxisAngle.hpp
//  TinyMath3D
//
//  Created by Mostafa on 03.05.25.
//

#include <Core/Vectors/Vector3.hpp>
#include <stdio.h>

using TinyMath3D::Vectors::Vector3;

namespace TinyMath3D::Rotations {
class AxisAngle {
  public:
	float theta = 0.0f;
	Vector3 n = Vector3(1, 0, 0);

	AxisAngle() = default;
	AxisAngle(float angle,const Vector3 &axis) : theta(angle), n(axis) {}
	// Create a normalized axis-angle (caller responsibility)
	static AxisAngle
	FromNormalized(float angle, const Vector3 &unitAxis) {
		return AxisAngle(angle, unitAxis);
	}

	// Create from non-normalized axis: auto normalize
	static AxisAngle
	FromUnnormalized(float angle, const Vector3 &axis) {
		auto unit = axis.Normalized(); // assume Vector3 has .Normalized()
		return AxisAngle(angle, unit);
	}

	/// @brief Returns a version with normalized axis.
	AxisAngle Normalized() const { return AxisAngle(theta, n.Normalized()); }

	/// @brief Validity check: theta is finite, axis has non-zero length
	bool IsValid(float epsilon = 1e-6f) const {
		return std::isfinite(theta) && n.Length() > epsilon;
	}

	/// @brief Returns identity rotation (zero angle)
	static AxisAngle Identity() {
		return AxisAngle(0.0f, Vector3(1.0f, 0.0f, 0.0f));
	}
};
} // namespace TinyMath3D::Rotations
