//
//  CartesianPolar2DConverter.hpp
//  TinyMath3D
//
//  Created by Mostafa on 08.04.25.
//
#pragma once

#include <cmath>
#include <stdio.h>
#include <vector>

namespace TinyMath3D::Converters {

struct Cartesian2DCoordinates {
	float x;
	float y;
};
struct Polar2DCoordinates {
	float r;
	float theta;
};
class CartesianPolar2DConverter {
  public:
	static Polar2DCoordinates ConvertToPolar2DCoordinates(const float &x,
	                                             const float &y);
	static Polar2DCoordinates ConvertToPola2DCoordinates(const Cartesian2DCoordinates &coordinates){
		return ConvertToPolar2DCoordinates(coordinates.x, coordinates.y);
	}
	
	static Cartesian2DCoordinates ConvertToCartesian2DCoordinates(const float &r,
	                                                      const float &theta);
	static Cartesian2DCoordinates ConvertToCartesian2DCoordinates(const Polar2DCoordinates &coordinates){
		return ConvertToCartesian2DCoordinates(coordinates.r, coordinates.theta);
	}
	
};

} // namespace TinyMath3D::Converters
