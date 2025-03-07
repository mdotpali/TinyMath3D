//
//  Vector3Test.cpp
//  TinyMath3D
//
//  Created by Mostafa on 07.03.25.
//

#include <Core/Vectors/Vector3.hpp>
#include <gtest/gtest.h>

using namespace TinyMath3D::Core::Vectors;
// Simple test to start with
TEST(Vector3Test, DefaultConstructor) {
	Vector3 v;
	EXPECT_FLOAT_EQ(v.x, 0.0f);
	EXPECT_FLOAT_EQ(v.y, 0.0f);
	EXPECT_FLOAT_EQ(v.z, 0.0f);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
