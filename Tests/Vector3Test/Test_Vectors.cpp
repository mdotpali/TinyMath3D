//
//  Test_Vectors.cpp
//  TinyMath3D
//
//  Created by Mostafa on 02.04.25.
//

#include <Core/Vectors/Vector3.hpp>
#include <Core/Vectors/Vector4.hpp>
#include <gtest/gtest.h>

using namespace TinyMath3D::Vectors;

	// -----------------------------------
	// Vector3 Tests
	// -----------------------------------
TEST(Vector3Test, DefaultConstructor) {
	Vector3 v;
	EXPECT_EQ(v.x, 0.0f);
	EXPECT_EQ(v.y, 0.0f);
	EXPECT_EQ(v.z, 0.0f);
}

TEST(Vector3Test, ParameterizedConstructor) {
	Vector3 v(1.0f, 2.0f, 3.0f);
	EXPECT_FLOAT_EQ(v.x, 1.0f);
	EXPECT_FLOAT_EQ(v.y, 2.0f);
	EXPECT_FLOAT_EQ(v.z, 3.0f);
}

TEST(Vector3Test, CopyConstructor) {
	Vector3 original(1.0f, 2.0f, 3.0f);
	Vector3 copy(original);
	EXPECT_FLOAT_EQ(copy.x, 1.0f);
	EXPECT_FLOAT_EQ(copy.y, 2.0f);
	EXPECT_FLOAT_EQ(copy.z, 3.0f);
}

TEST(Vector3Test, AssignmentOperator) {
	Vector3 v1(5.0f, 6.0f, 7.0f);
	Vector3 v2;
	v2 = v1;
	EXPECT_FLOAT_EQ(v2.x, 5.0f);
	EXPECT_FLOAT_EQ(v2.y, 6.0f);
	EXPECT_FLOAT_EQ(v2.z, 7.0f);
}

TEST(Vector3Test, AddOperator) {
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(4.0f, 5.0f, 6.0f);
	Vector3 sum = v1 + v2;
	EXPECT_FLOAT_EQ(sum.x, 5.0f);
	EXPECT_FLOAT_EQ(sum.y, 7.0f);
	EXPECT_FLOAT_EQ(sum.z, 9.0f);
}

TEST(Vector3Test, SubOperator) {
	Vector3 v1(4.0f, 5.0f, 6.0f);
	Vector3 v2(1.0f, 2.0f, 3.0f);
	Vector3 diff = v1 - v2;
	EXPECT_FLOAT_EQ(diff.x, 3.0f);
	EXPECT_FLOAT_EQ(diff.y, 3.0f);
	EXPECT_FLOAT_EQ(diff.z, 3.0f);
}

TEST(Vector3Test, DotProduct) {
	Vector3 v1(1.0f, 3.0f, -5.0f);
	Vector3 v2(4.0f, -2.0f, -1.0f);
	float dot = v1.Dot(v2);  // Dot should be 1*4 + 3*(-2) + (-5)*(-1) = 4 -6 + 5 = 3
	EXPECT_FLOAT_EQ(dot, 3.0f);
}

TEST(Vector3Test, CrossProduct) {
	Vector3 v1(1.0f, 2.0f, 3.0f);
	Vector3 v2(4.0f, 5.0f, 6.0f);
	Vector3 cross = v1.Cross(v2);
		// Cross product = ( (2*6 - 3*5), (3*4 - 1*6), (1*5 - 2*4) ) = (12 - 15, 12 - 6, 5 - 8) = (-3, 6, -3)
	EXPECT_FLOAT_EQ(cross.x, -3.0f);
	EXPECT_FLOAT_EQ(cross.y,  6.0f);
	EXPECT_FLOAT_EQ(cross.z, -3.0f);
}

TEST(Vector3Test, Length) {
	Vector3 v(3.0f, 4.0f, 12.0f);
	float length = v.Length(); // Should be sqrt(3^2 + 4^2 + 12^2) = sqrt(9+16+144) = sqrt(169) = 13
	EXPECT_FLOAT_EQ(length, 13.0f);
}

TEST(Vector3Test, Normalize) {
	Vector3 v(3.0f, 0.0f, 4.0f);
	v.Normalize(); // Should become (0.6, 0, 0.8)
	EXPECT_NEAR(v.x, 0.6f, 1e-5f);
	EXPECT_NEAR(v.y, 0.0f, 1e-5f);
	EXPECT_NEAR(v.z, 0.8f, 1e-5f);
}

	// -----------------------------------
	// Vector4 Tests
	// -----------------------------------
TEST(Vector4Test, DefaultConstructor) {
	Vector4 v;
	EXPECT_EQ(v.x, 0.0f);
	EXPECT_EQ(v.y, 0.0f);
	EXPECT_EQ(v.z, 0.0f);
	EXPECT_EQ(v.w, 0.0f);
}

TEST(Vector4Test, ParameterizedConstructor) {
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	EXPECT_FLOAT_EQ(v.x, 1.0f);
	EXPECT_FLOAT_EQ(v.y, 2.0f);
	EXPECT_FLOAT_EQ(v.z, 3.0f);
	EXPECT_FLOAT_EQ(v.w, 4.0f);
}

TEST(Vector4Test, AddOperator) {
	Vector4 v1(1.0f, 2.0f, 3.0f, 1.0f);
	Vector4 v2(4.0f, 5.0f, 6.0f, 0.0f);
	Vector4 sum = v1 + v2;
	EXPECT_FLOAT_EQ(sum.x, 5.0f);
	EXPECT_FLOAT_EQ(sum.y, 7.0f);
	EXPECT_FLOAT_EQ(sum.z, 9.0f);
	EXPECT_FLOAT_EQ(sum.w, 1.0f);
}

TEST(Vector4Test, DotProduct) {
	Vector4 v1(1.0f, 3.0f, -5.0f, 2.0f);
	Vector4 v2(4.0f, -2.0f, -1.0f, 2.0f);
	float dot = v1.Dot(v2);  // 1*4 + 3*(-2) + -5*(-1) + 2*2 = 4 -6 + 5 + 4 = 7
	EXPECT_FLOAT_EQ(dot, 7.0f);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
