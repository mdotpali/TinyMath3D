//
//  EulerAngle.hpp
//  TinyMath3D
//
//  Created by Mostafa on 03.05.25.
//

#pragma once
#include <stdio.h>
#include <limits>
#include <cmath>

namespace TinyMath3D::Rotations {
class EulerAngle {
  public:
	// Y-axis (yaw)
	float heading = 0.0f;

	// X-axis
	float pitch = 0.0f;

	// Z-axis(Bank)
	float bank = 0.0f;

	/// @brief Default constructor (identity rotation)
	 EulerAngle() = default;

	/// @brief Constructor from angles (in radians)
	 EulerAngle(float h, float p, float b)
	    : heading(h), pitch(p), bank(b) {}

	/// @brief Normalize all angles to be within [-pi, pi]
	inline void Normalize(){
		heading = NormalizeAngle(heading);
		pitch = NormalizeAngle(pitch);
		bank = NormalizeAngle(bank);
	}

	/// @brief Returns the canonical form (heading and bank in [-pi, pi], pitch
	/// in [-pi/2, pi/2])
	///        Handles gimbal lock by setting bank = 0 when pitch is ±90
	///        degrees.
	EulerAngle Canonicalized() const;

	
	/// @brief Checks whether this Euler triple is in canonical form.
	bool IsCanonical() const;

	/// @brief Constants for gimbal lock tolerance
	static constexpr float kGimbalLockTolerance = 1e-4f;

	/// @brief Constants for angle bounds (canonical form)
	static constexpr float kPi = 3.14159265358979323846f;
	static constexpr float kHalfPi = kPi * 0.5f;
	static constexpr float kTwoPi = kPi * 2.0f;

  private:
	
	inline static float wrapPi(float theta) {
		constexpr float kPi = 3.14159265358979323846f;
		constexpr float kTwoPi = 2.0f * kPi;
		theta += kPi;
		theta -= std::floor(theta / kTwoPi) * kTwoPi;
		theta -= kPi;
		return theta;
	}
	
	/// @brief Normalize a single angle to [-pi, pi]
	inline static float NormalizeAngle(float angle){
		return wrapPi(angle);
	}
};
} // namespace TinyMath3D::Rotations

