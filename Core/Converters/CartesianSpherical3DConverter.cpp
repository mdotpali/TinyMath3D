//
//  CartesianPolar3DConverter.cpp
//  TinyMath3D
//
//  Created by Mostafa on 10.04.25.
//

#include "CartesianSpherical3DConverter.hpp"
#include <cmath>

namespace TinyMath3D::Converters {
static Cartesian3DCoordinates SphericalToCartesian3D(const float &r,
                                                     const float &heading,
                                                     const float &pitch) {
	Cartesian3DCoordinates result;
	result.x = r * cos(pitch) * sin(heading);
	result.y = -r * sin(pitch);
	result.z = r * cos(pitch) * cos(heading);
	return result;
}
static Spherical3DCoordinates
CartesianToSpherical3D(const float &x, const float &y, const float &z) {
	Spherical3DCoordinates result;
	result.r = sqrt(x * x + y * y + z * z);
	if (result.r < std::numeric_limits<double>::epsilon())
		return {0.0, 0.0, 0.0};

	result.pitch = asin(-y / result.r);

	if (std::fabs(result.pitch) >= PI_OVER_TWO * 0.9999) {
		// Gimbal lock: heading is undefined when looking straight up/down
		result.heading = 0.0;
	} else {
		result.heading = atan2(x, z); // Left-handed: X → Z
	}
	result.heading = TinyMath3D::Converters::CartesianSpherical3DConverter::NormalizeAngle2Pi(result.heading);
	return result;
}

} // namespace TinyMath3D::Converters
