//
//  Vector3.hpp
//  TinyMath3D
//
//  Created by Mostafa on 07.03.25.
//

#pragma once

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
	
	//Operations
	float length() const;
	Vector3 normalized() const;
};

} // namespace Vectors
} // namespace Core
} // namespace TinMath3D
