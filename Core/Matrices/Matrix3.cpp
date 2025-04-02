//
//  Matrix3.cpp
//  TinyMath3D
//
//  Created by Mostafa on 01.04.25.
//

#include "Matrix3.hpp"
namespace TinyMath3D::Matrices {
Matrix3 Matrix3::Identity() {
	Matrix3 r;
	r.SetIdentity();
	return r;
}

Matrix3 Matrix3::Zero() {
	Matrix3 r;
	r.SetZero();
	return r;
}

Matrix3 Matrix3::Rotation2D(float radians) {
	// For a standard 2D rotation about Z (assuming column vectors and M *
	// v):
	//   cosθ  -sinθ  0
	//   sinθ   cosθ  0
	//    0      0    1
	//
	// In column-major:
	//   col0 = ( cosθ, sinθ, 0 )
	//   col1 = (-sinθ, cosθ, 0 )
	//   col2 = (  0,     0,   1 )
	float c = cosf(radians);
	float s = sinf(radians);
	return Matrix3(c, s, 0.f,    // first column
	               -s, c, 0.f,   // second column
	               0.f, 0.f, 1.f // third column
	);
}

Matrix3 Matrix3::Scale2D(float sx, float sy) {
	//  sx   0   0
	//   0  sy   0
	//   0   0   1
	//
	// col0 = (sx, 0, 0)
	// col1 = (0, sy, 0)
	// col2 = (0,  0, 1)
	return Matrix3(sx, 0.f, 0.f, 0.f, sy, 0.f, 0.f, 0.f, 1.f);
}

Matrix3 Matrix3::Translation2D(float tx, float ty) {
	// 1  0  tx
	// 0  1  ty
	// 0  0  1
	//
	// col0 = (1, 0, 0)
	// col1 = (0, 1, 0)
	// col2 = (tx,ty,1)
	return Matrix3(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, tx, ty, 1.f);
}

Matrix3 Matrix3::operator*(const Matrix3 &rhs) const {
	Matrix3 result;

	// For column-major: M( row, col ) = sum over k of this(row, k)*rhs(k,
	// col). But row, col => index = col*3 + row.
	for (int col = 0; col < 3; ++col) {
		for (int row = 0; row < 3; ++row) {
			float sum = 0.f;
			for (int k = 0; k < 3; ++k) {
				// this(row,k) -> m[k*3 + row]
				// rhs(k,col)  -> rhs.m[col*3 + k]
				sum += m[k * 3 + row] * rhs.m[col * 3 + k];
			}
			result.m[col * 3 + row] = sum;
		}
	}
	return result;
}

float Matrix3::Determinant() const {
	// We'll map them more explicitly for clarity:
	//   a00 = m[0] => M(0,0)
	//   a10 = m[1] => M(1,0)
	//   a20 = m[2] => M(2,0)
	//   a01 = m[3] => M(0,1)
	//   a11 = m[4] => M(1,1)
	//   a21 = m[5] => M(2,1)
	//   a02 = m[6] => M(0,2)
	//   a12 = m[7] => M(1,2)
	//   a22 = m[8] => M(2,2)

	float a00 = m[0], a10 = m[1], a20 = m[2];
	float a01 = m[3], a11 = m[4], a21 = m[5];
	float a02 = m[6], a12 = m[7], a22 = m[8];

	return a00 * (a11 * a22 - a12 * a21) - a01 * (a10 * a22 - a12 * a20) +
	       a02 * (a10 * a21 - a11 * a20);
}

bool Matrix3::Invert() {
	float det = Determinant();
	if (fabsf(det) < FLT_EPSILON) {
		return false; // Not invertible
	}

	float invDet = 1.f / det;

	float a00 = m[0], a10 = m[1], a20 = m[2];
	float a01 = m[3], a11 = m[4], a21 = m[5];
	float a02 = m[6], a12 = m[7], a22 = m[8];

	Matrix3 temp;
	// Cofactor matrix transposed (aka adjugate)
	temp.m[0] = (a11 * a22 - a12 * a21) * invDet;  // +c00
	temp.m[3] = -(a01 * a22 - a02 * a21) * invDet; // -c01
	temp.m[6] = (a01 * a12 - a02 * a11) * invDet;  // +c02

	temp.m[1] = -(a10 * a22 - a20 * a12) * invDet; // -c10
	temp.m[4] = (a00 * a22 - a20 * a02) * invDet;  // +c11
	temp.m[7] = -(a00 * a12 - a10 * a02) * invDet; // -c12

	temp.m[2] = (a10 * a21 - a20 * a11) * invDet;  // +c20
	temp.m[5] = -(a00 * a21 - a20 * a01) * invDet; // -c21
	temp.m[8] = (a00 * a11 - a10 * a01) * invDet;  // +c22

	*this = temp;
	return true;
}

Matrix3 Matrix3::GetInverse(bool *success) const {
	Matrix3 r(*this);
	bool ok = r.Invert();
	if (success)
		*success = ok;
	return r;
}
} // namespace TinyMath3D::Matrices
