//
//  EulerAngle.cpp
//  TinyMath3D
//
//  Created by Mostafa on 03.05.25.
//

#include "EulerAngle.hpp"
#include <cmath>
#include <algorithm>

namespace TinyMath3D::Rotations {
	

	EulerAngle EulerAngle::Canonicalized() const{
		EulerAngle result = *this;
		result.heading = NormalizeAngle(result.heading);
		result.pitch = std::clamp(result.pitch, -kHalfPi, kHalfPi);
		
		// Gimbal lock case: pitch == ±90°
		if(std::abs(result.pitch) >= (kHalfPi - kGimbalLockTolerance)){
			result.bank = 0;
		}else{
			result.bank = NormalizeAngle(result.bank);
		}
		
		return result;
	}
}
