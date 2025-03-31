//
//  Matrix4.cpp
//  TinyMath3D
//
//  Created by Mostafa on 27.03.25.
//
#include "Matrix4.hpp"
#include <Core/Vectors/Vector3.hpp>
#include <algorithm>

namespace TinyMath3D::Matrices {

//----------------------------------------------------------------------------------------
// Static Functions
//----------------------------------------------------------------------------------------
// fovRadians is the vertical field of view in radians.
// aspect = width / height
// This uses a standard right-handed perspective projection
[[maybe_unused]] static Matrix4 Perspective(float fovRadians, float aspect,
                                            float zNear, float zFar) {
	Matrix4 mat = Matrix4::Zero();

	float f = 1.0f / std::tan(fovRadians * 0.5f);
	float range = zFar - zNear;

	mat.m[0] = f / aspect;
	mat.m[5] = f;
	mat.m[10] = -(zFar + zNear) / range;
	mat.m[11] = -1.0f;
	mat.m[14] = -2.0f * zFar * zNear / range;
	return mat;
}

[[maybe_unused]] static Matrix4 Orthographic(float left, float right,
                                             float bottom, float top,
                                             float zNear, float zFar) {
	Matrix4 mat = Matrix4::Identity();

	float w = right - left;
	float h = top - bottom;
	float d = zFar - zNear;

	mat.m[0] = 2.0f / w;
	mat.m[5] = 2.0f / h;
	mat.m[10] = -2.0f / d;
	mat.m[12] = -(right + left) / w;
	mat.m[13] = -(top + bottom) / h;
	mat.m[14] = -(zFar + zNear) / d;
	return mat;
}

// right handed look at
[[maybe_unused]] static Matrix4 LookAt(const Vectors::Vector3 &eye,
                                       const Vectors::Vector3 &center,
                                       const Vectors::Vector3 &up) {
	Vectors::Vector3 zAxis = {eye.x - center.x, eye.y - center.y,
	                          eye.z - eye.z};
	// normalize zAxis
	zAxis.normalize();

	// cross(up, zAxis) => xAxis
	Vectors::Vector3 xAxis = up.cross(zAxis);
	// normalize zAxis
	xAxis.normalize();

	// cross(zAxis, xAxis) => yAxis
	Vectors::Vector3 yAxis = zAxis.cross(xAxis);

	// Build the look-at matrix in column-major
	Matrix4 mat = Matrix4::Identity();

	// X axis column
	mat.m[0] = xAxis.x;
	mat.m[1] = xAxis.y;
	mat.m[2] = xAxis.z;

	// Y axis column
	mat.m[4] = yAxis.x;
	mat.m[5] = yAxis.y;
	mat.m[6] = yAxis.z;

	// Z axis column
	mat.m[8] = zAxis.x;
	mat.m[9] = zAxis.y;
	mat.m[10] = zAxis.z;

	// Now set translation. Because it’s column-major,
	// the translation is in elements 12,13,14, adjusted to
	// "move" eye to origin.
	mat.m[12] = -(xAxis.x * eye.x + xAxis.y * eye.y + xAxis.z * eye.z);
	mat.m[13] = -(yAxis.x * eye.x + yAxis.y * eye.y + yAxis.z * eye.z);
	mat.m[14] = -(zAxis.x * eye.x + zAxis.y * eye.y + zAxis.z * eye.z);

	return mat;
}

//----------------------------------------------------------------------------------------
// Multiplications
//----------------------------------------------------------------------------------------
// Matrix4 * Matrix4
Matrix4 Matrix4::operator*(const Matrix4 &rhs) const {
	Matrix4 result;
	// Column-major multiplication formula
	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			float sum = 0.0f;
			for (int i = 0; i < 4; ++i) {
				sum += m[col * 4 + i] * rhs.m[i * 4 + row];
			}
			result.m[col * 4 + row] = sum;
		}
	}
	return result;
}
Matrix4 &Matrix4::operator*=(const Matrix4 &rhs) {
	*this = (*this) * rhs;
	return *this;
}

//----------------------------------------------------------------------------------------
// Transpose, Inversion
//----------------------------------------------------------------------------------------
Matrix4 Matrix4::Transposed() const {
	Matrix4 out;
	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			out.m[row * 4 + col] = m[col * 4 + row];
		}
	}
	return out;
}

// Invert for affine transormations: rotation, scale, translation (no
// perspective projection)
Matrix4 Matrix4::InvertAffine(const Matrix4 &mat) {
	Matrix4 inv;

	// Step 1: Invert the 3x3 rotation/scale portion (transpose if it's
	// orthonormal)
	inv.m[0] = mat.m[0];
	inv.m[1] = mat.m[4];
	inv.m[2] = mat.m[8];
	inv.m[3] = 0.0f;
	inv.m[4] = mat.m[1];
	inv.m[5] = mat.m[5];
	inv.m[6] = mat.m[9];
	inv.m[7] = 0.0f;
	inv.m[8] = mat.m[2];
	inv.m[9] = mat.m[6];
	inv.m[10] = mat.m[10];
	inv.m[11] = 0.0f;

	// Step 2: Invert the translation
	float tx = mat.m[12];
	float ty = mat.m[13];
	float tz = mat.m[14];

	inv.m[12] = -(inv.m[0] * tx + inv.m[4] * ty + inv.m[8] * tz);
	inv.m[13] = -(inv.m[1] * tx + inv.m[5] * ty + inv.m[9] * tz);
	inv.m[14] = -(inv.m[2] * tx + inv.m[6] * ty + inv.m[10] * tz);
	inv.m[15] = 1.0f;

	return inv;
}

Matrix4 Matrix4::InvertFullCofactorMethod(const Matrix4 &mat) {
		// We'll use a standard cofactor-based approach. It's large, so keep it out of inline.
	const float* src = mat.m;
	Matrix4 inv;
	float* dst = inv.m;
	
		// Calculate cofactors (a fairly standard approach):
	dst[0] = src[5] * src[10] * src[15] -
	src[5] * src[11] * src[14] -
	src[9] * src[6]  * src[15] +
	src[9] * src[7]  * src[14] +
	src[13]* src[6]  * src[11] -
	src[13]* src[7]  * src[10];
	
	dst[4] = -src[4] * src[10] * src[15] +
	src[4] * src[11] * src[14] +
	src[8] * src[6]  * src[15] -
	src[8] * src[7]  * src[14] -
	src[12]* src[6]  * src[11] +
	src[12]* src[7]  * src[10];
	
	dst[8] = src[4] * src[9] * src[15] -
	src[4] * src[11]* src[13] -
	src[8] * src[5] * src[15] +
	src[8] * src[7] * src[13] +
	src[12]* src[5] * src[11] -
	src[12]* src[7] * src[9];
	
	dst[12] = -src[4] * src[9] * src[14] +
	src[4] * src[10]* src[13] +
	src[8] * src[5] * src[14] -
	src[8] * src[6] * src[13] -
	src[12]* src[5] * src[10] +
	src[12]* src[6] * src[9];
	
	dst[1] = -src[1] * src[10]* src[15] +
	src[1] * src[11]* src[14] +
	src[9] * src[2] * src[15] -
	src[9] * src[3] * src[14] -
	src[13]* src[2] * src[11] +
	src[13]* src[3] * src[10];
	
	dst[5] = src[0] * src[10]* src[15] -
	src[0] * src[11]* src[14] -
	src[8] * src[2] * src[15] +
	src[8] * src[3] * src[14] +
	src[12]* src[2] * src[11] -
	src[12]* src[3] * src[10];
	
	dst[9] = -src[0] * src[9] * src[15] +
	src[0] * src[11]* src[13] +
	src[8] * src[1] * src[15] -
	src[8] * src[3] * src[13] -
	src[12]* src[1] * src[11] +
	src[12]* src[3] * src[9];
	
	dst[13] = src[0] * src[9] * src[14] -
	src[0] * src[10]* src[13] -
	src[8] * src[1] * src[14] +
	src[8] * src[2] * src[13] +
	src[12]* src[1] * src[10] -
	src[12]* src[2] * src[9];
	
	dst[2] = src[1] * src[6] * src[15] -
	src[1] * src[7] * src[14] -
	src[5] * src[2] * src[15] +
	src[5] * src[3] * src[14] +
	src[13]* src[2] * src[7]  -
	src[13]* src[3] * src[6];
	
	dst[6] = -src[0] * src[6] * src[15] +
	src[0] * src[7] * src[14] +
	src[4] * src[2] * src[15] -
	src[4] * src[3] * src[14] -
	src[12]* src[2] * src[7]  +
	src[12]* src[3] * src[6];
	
	dst[10] = src[0] * src[5] * src[15] -
	src[0] * src[7] * src[13] -
	src[4] * src[1] * src[15] +
	src[4] * src[3] * src[13] +
	src[12]* src[1] * src[7]  -
	src[12]* src[3] * src[5];
	
	dst[14] = -src[0] * src[5] * src[14] +
	src[0] * src[6] * src[13] +
	src[4] * src[1] * src[14] -
	src[4] * src[2] * src[13] -
	src[12]* src[1] * src[6]  +
	src[12]* src[2] * src[5];
	
	dst[3] = -src[1] * src[6] * src[11] +
	src[1] * src[7] * src[10] +
	src[5] * src[2] * src[11] -
	src[5] * src[3] * src[10] -
	src[9] * src[2] * src[7]  +
	src[9] * src[3] * src[6];
	
	dst[7] = src[0] * src[6] * src[11] -
	src[0] * src[7] * src[10] -
	src[4] * src[2] * src[11] +
	src[4] * src[3] * src[10] +
	src[8] * src[2] * src[7]  -
	src[8] * src[3] * src[6];
	
	dst[11] = -src[0] * src[5] * src[11] +
	src[0] * src[7] * src[9]  +
	src[4] * src[1] * src[11] -
	src[4] * src[3] * src[9]  -
	src[8] * src[1] * src[7]  +
	src[8] * src[3] * src[5];
	
	dst[15] = src[0] * src[5] * src[10] -
	src[0] * src[6] * src[9]  -
	src[4] * src[1] * src[10] +
	src[4] * src[2] * src[9]  +
	src[8] * src[1] * src[6]  -
	src[8] * src[2] * src[5];
	
		// determinant
	float det = src[0]*dst[0] + src[1]*dst[4] + src[2]*dst[8] + src[3]*dst[12];
	if(std::fabs(det) < 1e-6f) {
			// Not invertible
		return Identity();
	}
	
	float invDet = 1.0f / det;
	for(int i = 0; i < 16; i++)
		dst[i] *= invDet;
	
	return inv;
}
} // namespace TinyMath3D::Matrices
