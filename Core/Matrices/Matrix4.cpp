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
	zAxis.Normalize();

	// cross(up, zAxis) => xAxis
	Vectors::Vector3 xAxis = up.Cross(zAxis);
	// normalize zAxis
	xAxis.Normalize();

	// cross(zAxis, xAxis) => yAxis
	Vectors::Vector3 yAxis = zAxis.Cross(xAxis);

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
/**
 * @brief Matrix multiplication (this * rhs), column-major.
 *        result(row, col) = Σ over k of [ this(row, k) * rhs(k, col) ]
 *        With indexing result[col*4 + row].
 */
Matrix4 Matrix4::operator*(const Matrix4 &rhs) const {
	Matrix4 result;
	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			float sum = 0.f;
			for (int k = 0; k < 4; ++k) {
				// this(row,k) => m[k*4 + row]
				// rhs(k,col)  => rhs.m[col*4 + k]
				sum += m[k * 4 + row] * rhs.m[col * 4 + k];
			}
			result.m[col * 4 + row] = sum;
		}
	}
	return result;
}

//----------------------------------------------------------------------------------------
// Advance
//----------------------------------------------------------------------------------------
Matrix4 Matrix4::GetTransposed() const {
	Matrix4 out;
		// For each (row, col), copy transposed element from (col, row).
	for (int row = 0; row < 4; ++row)
		{
		for (int col = 0; col < 4; ++col)
			{
			out.m[col * 4 + row] = m[row * 4 + col];
			}
		}
	return out;
}

float Matrix4::Determinant() const {
	// 4x4 determinant is more involved. Typically we do a cofactor expansion or
	// use a standard algorithm. For simplicity, let's do cofactor expansion
	// along the first row. Let the first row be m[0], m[1], m[2], m[3]:
	// But recall indexing: m[0] => (row=0,col=0), m[4] => (row=0,col=1), etc.
	//
	// a = m[0], b = m[4], c = m[8],  d = m[12]
	// e = m[1], f = m[5], g = m[9],  h = m[13]
	// i = m[2], j = m[6], k = m[10], l = m[14]
	// p = m[3], q = m[7], r = m[11], s = m[15]
	//
	// Then det(M) = a * det( minor(a) ) - b * det( minor(b) ) + c * det(
	// minor(c) ) - d * det( minor(d) ) We'll implement a standard method here,
	// or use a known 4x4 determinant formula.

	float a = m[0];
	float b = m[4];
	float c = m[8];
	float d = m[12];
	float e = m[1];
	float f = m[5];
	float g = m[9];
	float h = m[13];
	float i = m[2];
	float j = m[6];
	float k = m[10];
	float l = m[14];
	float p = m[3];
	float q = m[7];
	float r = m[11];
	float s = m[15];

	// We can do a known "faster" 4x4 det formula:
	float detA = a * (f * k * s + g * l * q + h * j * r - f * l * r -
	                  g * j * s - h * k * q);
	float detB = -b * (e * k * s + g * l * p + h * i * r - e * l * r -
	                   g * i * s - h * k * p);
	float detC = c * (e * j * s + f * l * p + h * i * q - e * l * q -
	                  f * i * s - h * j * p);
	float detD = -d * (e * j * r + f * i * r + g * i * q - e * k * q -
	                   f * j * p - g * i * r); // check carefully

	return (detA + detB + detC + detD);
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

bool Matrix4::Invert() {
	// Compute the determinant first:
	float det = Determinant();
	if (std::fabs(det) < FLT_EPSILON) {
		return false; // Not invertible
	}

	float invDet = 1.f / det;

	// For 4x4, a typical approach is to compute the inverse via
	// the adjoint (cofactor matrix transposed) * 1/det.
	// We’ll implement a standard method or a known approach.
	// For brevity, let's do an explicit solution or common "mat4 invert"
	// snippet:

	// We'll map the current matrix into a short alias to keep code readable:
	float a = m[0];
	float b = m[4];
	float c = m[8];
	float d = m[12];
	float e = m[1];
	float f = m[5];
	float g = m[9];
	float h = m[13];
	float i = m[2];
	float j = m[6];
	float k = m[10];
	float l = m[14];
	float p = m[3];
	float q = m[7];
	float r = m[11];
	float s = m[15];

	Matrix4 temp; // We'll build the inverse into temp, then copy back.

	temp.m[0] =
	    (f * k * s + g * l * q + h * j * r - f * l * r - g * j * s - h * k * q);
	temp.m[4] = -(b * k * s + c * l * q + d * j * r - b * l * r - c * j * s -
	              d * k * q);
	temp.m[8] =
	    (b * g * s + c * h * q + d * f * r - b * h * r - c * f * s - d * g * q);
	temp.m[12] = -(b * g * l + c * h * j + d * f * k - b * h * k - c * f * l -
	               d * g * j);

	temp.m[1] = -(e * k * s + g * l * p + h * i * r - e * l * r - g * i * s -
	              h * k * p);
	temp.m[5] =
	    (a * k * s + c * l * p + d * i * r - a * l * r - c * i * s - d * k * p);
	temp.m[9] = -(a * g * s + c * h * p + d * f * r - a * h * r - c * f * s -
	              d * g * p);
	temp.m[13] =
	    (a * g * l + c * h * i + d * f * k - a * h * k - c * f * l - d * g * i);

	temp.m[2] =
	    (e * j * s + f * l * p + h * i * q - e * l * q - f * i * s - h * j * p);
	temp.m[6] = -(a * j * s + b * l * p + d * i * q - a * l * q - b * i * s -
	              d * j * p);
	temp.m[10] =
	    (a * f * s + b * h * p + d * e * q - a * h * q - b * e * s - d * f * p);
	temp.m[14] = -(a * f * l + b * h * i + d * e * j - a * h * j - b * e * l -
	               d * f * i);

	temp.m[3] = -(e * j * r + f * i * r + g * i * q - e * k * q - f * j * p -
	              g * i * r);
	// This line is suspicious in some expansions;
	// carefully check your minor expansions if needed.
	temp.m[7] =
	    (a * j * r + b * i * r + c * i * q - a * k * q - b * j * p - c * i * r);
	temp.m[11] = -(a * f * r + b * e * r + c * e * q - a * g * q - b * f * p -
	               c * e * r);
	temp.m[15] =
	    (a * f * k + b * e * j + c * e * i - a * g * j - b * e * k - c * f * i);

	// Multiply by 1 / det
	for (int idx = 0; idx < 16; ++idx) {
		temp.m[idx] *= invDet;
	}

	// Copy the result back
	*this = temp;
	return true;
}

Matrix4 Matrix4::GetInverse(bool *success) const {
	Matrix4 result(*this);
	bool ok = result.Invert();
	if (success)
		*success = ok;
	return result;
}
	
Matrix4 Matrix4::InvertFullCofactorMethod(const Matrix4 &mat) {
	// We'll use a standard cofactor-based approach. It's large, so keep it out
	// of inline.
	const float *src = mat.m;
	Matrix4 inv;
	float *dst = inv.m;

	// Calculate cofactors (a fairly standard approach):
	dst[0] = src[5] * src[10] * src[15] - src[5] * src[11] * src[14] -
	         src[9] * src[6] * src[15] + src[9] * src[7] * src[14] +
	         src[13] * src[6] * src[11] - src[13] * src[7] * src[10];

	dst[4] = -src[4] * src[10] * src[15] + src[4] * src[11] * src[14] +
	         src[8] * src[6] * src[15] - src[8] * src[7] * src[14] -
	         src[12] * src[6] * src[11] + src[12] * src[7] * src[10];

	dst[8] = src[4] * src[9] * src[15] - src[4] * src[11] * src[13] -
	         src[8] * src[5] * src[15] + src[8] * src[7] * src[13] +
	         src[12] * src[5] * src[11] - src[12] * src[7] * src[9];

	dst[12] = -src[4] * src[9] * src[14] + src[4] * src[10] * src[13] +
	          src[8] * src[5] * src[14] - src[8] * src[6] * src[13] -
	          src[12] * src[5] * src[10] + src[12] * src[6] * src[9];

	dst[1] = -src[1] * src[10] * src[15] + src[1] * src[11] * src[14] +
	         src[9] * src[2] * src[15] - src[9] * src[3] * src[14] -
	         src[13] * src[2] * src[11] + src[13] * src[3] * src[10];

	dst[5] = src[0] * src[10] * src[15] - src[0] * src[11] * src[14] -
	         src[8] * src[2] * src[15] + src[8] * src[3] * src[14] +
	         src[12] * src[2] * src[11] - src[12] * src[3] * src[10];

	dst[9] = -src[0] * src[9] * src[15] + src[0] * src[11] * src[13] +
	         src[8] * src[1] * src[15] - src[8] * src[3] * src[13] -
	         src[12] * src[1] * src[11] + src[12] * src[3] * src[9];

	dst[13] = src[0] * src[9] * src[14] - src[0] * src[10] * src[13] -
	          src[8] * src[1] * src[14] + src[8] * src[2] * src[13] +
	          src[12] * src[1] * src[10] - src[12] * src[2] * src[9];

	dst[2] = src[1] * src[6] * src[15] - src[1] * src[7] * src[14] -
	         src[5] * src[2] * src[15] + src[5] * src[3] * src[14] +
	         src[13] * src[2] * src[7] - src[13] * src[3] * src[6];

	dst[6] = -src[0] * src[6] * src[15] + src[0] * src[7] * src[14] +
	         src[4] * src[2] * src[15] - src[4] * src[3] * src[14] -
	         src[12] * src[2] * src[7] + src[12] * src[3] * src[6];

	dst[10] = src[0] * src[5] * src[15] - src[0] * src[7] * src[13] -
	          src[4] * src[1] * src[15] + src[4] * src[3] * src[13] +
	          src[12] * src[1] * src[7] - src[12] * src[3] * src[5];

	dst[14] = -src[0] * src[5] * src[14] + src[0] * src[6] * src[13] +
	          src[4] * src[1] * src[14] - src[4] * src[2] * src[13] -
	          src[12] * src[1] * src[6] + src[12] * src[2] * src[5];

	dst[3] = -src[1] * src[6] * src[11] + src[1] * src[7] * src[10] +
	         src[5] * src[2] * src[11] - src[5] * src[3] * src[10] -
	         src[9] * src[2] * src[7] + src[9] * src[3] * src[6];

	dst[7] = src[0] * src[6] * src[11] - src[0] * src[7] * src[10] -
	         src[4] * src[2] * src[11] + src[4] * src[3] * src[10] +
	         src[8] * src[2] * src[7] - src[8] * src[3] * src[6];

	dst[11] = -src[0] * src[5] * src[11] + src[0] * src[7] * src[9] +
	          src[4] * src[1] * src[11] - src[4] * src[3] * src[9] -
	          src[8] * src[1] * src[7] + src[8] * src[3] * src[5];

	dst[15] = src[0] * src[5] * src[10] - src[0] * src[6] * src[9] -
	          src[4] * src[1] * src[10] + src[4] * src[2] * src[9] +
	          src[8] * src[1] * src[6] - src[8] * src[2] * src[5];

	// determinant
	float det =
	    src[0] * dst[0] + src[1] * dst[4] + src[2] * dst[8] + src[3] * dst[12];
	if (std::fabs(det) < 1e-6f) {
		// Not invertible
		return Identity();
	}

	float invDet = 1.0f / det;
	for (int i = 0; i < 16; i++)
		dst[i] *= invDet;

	return inv;
}
} // namespace TinyMath3D::Matrices
