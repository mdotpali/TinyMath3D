//
//  Matrix3.hpp
//  TinyMath3D
//
//  Created by Mostafa on 01.04.25.
//

#pragma once
#include <cfloat>
#include <cmath>
#include <stdio.h>

namespace TinyMath3D::Matrices {
/**
 * @class Matrix3
 * @brief A 3x3 floating-point matrix stored in column-major order.
 *
 * In memory, m[] is:
 *   [0] (row=0, col=0)
 *   [1] (row=1, col=0)
 *   [2] (row=2, col=0)
 *   [3] (row=0, col=1)
 *   [4] (row=1, col=1)
 *   [5] (row=2, col=1)
 *   [6] (row=0, col=2)
 *   [7] (row=1, col=2)
 *   [8] (row=2, col=2)
 */
class Matrix3 {
  public:
	/// The 9 floats in column-major order
	float m[9];

	/**
	 * @brief Default constructor. Sets to Identity.
	 */
	inline Matrix3() { SetIdentity(); }

	/**
	 * @brief Explicit column-major constructor.
	 *        The parameters come in column order, top-to-bottom.
	 *
	 *        That is:
	 *          col0 = (c0r0, c0r1, c0r2)
	 *          col1 = (c1r0, c1r1, c1r2)
	 *          col2 = (c2r0, c2r1, c2r2)
	 *
	 *        So m[0] = c0r0, m[1] = c0r1, m[2] = c0r2, etc.
	 */
	inline Matrix3(float c0r0, float c0r1, float c0r2, float c1r0, float c1r1,
	               float c1r2, float c2r0, float c2r1, float c2r2) {
		m[0] = c0r0;
		m[1] = c0r1;
		m[2] = c0r2; // First column
		m[3] = c1r0;
		m[4] = c1r1;
		m[5] = c1r2; // Second column
		m[6] = c2r0;
		m[7] = c2r1;
		m[8] = c2r2; // Third column
	}

	/**
	 * @brief Construct from a raw array of 9 floats (already in column-major
	 * order).
	 */
	inline explicit Matrix3(const float *values) {
		for (int i = 0; i < 9; ++i)
			m[i] = values[i];
	}

	// ------------------------------------------------------------------------
	// Static creators
	// ------------------------------------------------------------------------
	static Matrix3 Identity();
	static Matrix3 Zero();

	/**
	 * @brief Create a 2D rotation matrix (rotates around Z axis).
	 * @param radians The rotation angle in radians.
	 */
	static Matrix3 Rotation2D(float radians);

	/**
	 * @brief Create a 2D scaling matrix.
	 */
	static Matrix3 Scale2D(float sx, float sy);

	/**
	 * @brief Create a 2D translation matrix.
	 */
	static Matrix3 Translation2D(float tx, float ty);

	// ------------------------------------------------------------------------
	// Basic set/reset
	// ------------------------------------------------------------------------
	inline void SetIdentity() {
		m[0] = 1.f;
		m[1] = 0.f;
		m[2] = 0.f;
		m[3] = 0.f;
		m[4] = 1.f;
		m[5] = 0.f;
		m[6] = 0.f;
		m[7] = 0.f;
		m[8] = 1.f;
	}

	inline void SetZero() {
		for (int i = 0; i < 9; ++i)
			m[i] = 0.f;
	}

	// ------------------------------------------------------------------------
	// Indexing
	// ------------------------------------------------------------------------
	/**
	 * @brief Read/write a single element (column-major).
	 * @param idx [0..8]
	 */
	inline float &operator[](int idx) { return m[idx]; }
	inline float operator[](int idx) const { return m[idx]; }

	// ------------------------------------------------------------------------
	// Comparison
	// ------------------------------------------------------------------------
	inline bool operator==(const Matrix3 &rhs) const {
		const float EPS = 1e-6f;
		for (int i = 0; i < 9; ++i)
			if (fabsf(m[i] - rhs.m[i]) > EPS)
				return false;
		return true;
	}
	inline bool operator!=(const Matrix3 &rhs) const { return !(*this == rhs); }

	// ------------------------------------------------------------------------
	// Arithmetic
	// ------------------------------------------------------------------------
	inline Matrix3 operator+(const Matrix3 &rhs) const {
		Matrix3 result;
		for (int i = 0; i < 9; ++i)
			result.m[i] = m[i] + rhs.m[i];
		return result;
	}
	inline Matrix3 &operator+=(const Matrix3 &rhs) {
		for (int i = 0; i < 9; ++i)
			m[i] += rhs.m[i];
		return *this;
	}

	inline Matrix3 operator-(const Matrix3 &rhs) const {
		Matrix3 result;
		for (int i = 0; i < 9; ++i)
			result.m[i] = m[i] - rhs.m[i];
		return result;
	}
	inline Matrix3 &operator-=(const Matrix3 &rhs) {
		for (int i = 0; i < 9; ++i)
			m[i] -= rhs.m[i];
		return *this;
	}

	inline Matrix3 operator*(float s) const {
		Matrix3 result;
		for (int i = 0; i < 9; ++i)
			result.m[i] = m[i] * s;
		return result;
	}
	inline Matrix3 &operator*=(float s) {
		for (int i = 0; i < 9; ++i)
			m[i] *= s;
		return *this;
	}

	/**
	 * @brief Matrix multiplication (this * rhs).
	 */
	Matrix3 operator*(const Matrix3 &rhs) const;

	inline Matrix3 &operator*=(const Matrix3 &rhs) {
		*this = (*this) * rhs;
		return *this;
	}

	// ------------------------------------------------------------------------
	// Advanced (implemented in .cpp)
	// ------------------------------------------------------------------------
	float Determinant() const;
	bool Invert();
	Matrix3 GetInverse(bool *success = nullptr) const;
};

/**
 * @brief Allows scalar * Matrix3 from the left.
 */
inline Matrix3 operator*(float s, const Matrix3 &mat) { return mat * s; }
} // namespace TinyMath3D::Matrices
