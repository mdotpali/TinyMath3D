//
//  CartesianPolar3DConverter.hpp
//  TinyMath3D
//
//  Created by Mostafa on 10.04.25.
//
#pragma once

#include <stdio.h>

namespace TinyMath3D::Converters {

constexpr float PI = 3.14159265358979323846;
constexpr float TWO_PI = 2.0 * PI;
constexpr float PI_OVER_TWO = PI / 2.0;

struct Cartesian3DCoordinates {
	float x;
	float y;
	float z;
};
struct Spherical3DCoordinates {
	float r;
	float heading;
	float pitch;
};
class CartesianSpherical3DConverter {
public:
	static Cartesian3DCoordinates SphericalToCartesian3D(const float &r,
	                                                     const float &heading,
	                                                     const float &pitch);
	inline static Cartesian3DCoordinates
	SphericalToCartesian3D(const Spherical3DCoordinates &coordinates) {
		return SphericalToCartesian3D(coordinates.r, coordinates.heading,
		                              coordinates.pitch);
	}

	static Spherical3DCoordinates
	CartesianToSpherical3D(const float &x, const float &y, const float &z);
	static Spherical3DCoordinates
	CartesianToSpherical3D(const Cartesian3DCoordinates);

	inline static float NormalizeAngle2Pi(float angle) {
		while (angle < 0.0)
			angle += TWO_PI;
		while (angle >= TWO_PI)
			angle -= TWO_PI;
		return angle;
	}
};
} // namespace TinyMath3D::Converters
