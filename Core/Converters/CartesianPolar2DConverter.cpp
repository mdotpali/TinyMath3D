//
//  CartesianPolar2DConverter.cpp
//  TinyMath3D
//
//  Created by Mostafa on 08.04.25.
//

#include "CartesianPolar2DConverter.hpp"
#include <cmath>
#include <vector>

namespace TinyMath3D::Converters {

static Polar2DCoordinates ConvertToPolarCoordinates(const float &x, const float &y) {
	Polar2DCoordinates result;
	if (x == 0 && y == 0) {
		result.r = 0;
		result.theta = 0;
	} else {
		result.r = sqrt((x * x) + (y * y));
		result.theta = atan2(y, x);
	}
	return result;
}

static Cartesian2DCoordinates ConvertToCartesianCoordinates(const float &r,
                                                   const float &theta) {
	Cartesian2DCoordinates result{r * cos(theta), r * sin(theta)};
	// result.x = r * cos(theta);
	// result.y = r * sin(theta);
	return result;
}

} // namespace TinyMath3D::Converters
