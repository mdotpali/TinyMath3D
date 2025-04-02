//
//  Matrix4.hpp
//  TinyMath3D
//
//  Created by Mostafa on 27.03.25.
//
#pragma once

#include <Core/Vectors/Vector3.hpp>
#include <cmath>
#include <cfloat>

namespace TinyMath3D::Matrices {

/**
 * @class Matrix4
 * @brief A 4x4 float matrix in column-major order.
 *
 * Column-major means:
 *   The first 4 floats (m[0..3]) form the first column (rows = 0..3),
 *   The next 4 floats (m[4..7]) form the second column,
 *   The next 4 floats (m[8..11]) form the third column,
 *   The last 4 floats (m[12..15]) form the fourth column.
 *
 * Thus element (row, col) is stored at m[col * 4 + row].
 * For standard 3D transforms, translation is in m[12], m[13], m[14],
 * and the bottom row is stored in m[3], m[7], m[11], m[15].
 */
class Matrix4 {

  public:
	/// The 16 floats that make up this matrix, in column-major order.
	float m[16];

	//----------------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------------
	/**
	 * @brief Default constructor. Sets this matrix to the identity matrix.
	 */
	inline Matrix4() { SetIdentity(); }

	/**
	 * @brief Construct from explicit float values, in column-major order.
	 *
	 *        The parameters represent columns:
	 *          col0 = (c0r0, c0r1, c0r2, c0r3)
	 *          col1 = (c1r0, c1r1, c1r2, c1r3)
	 *          col2 = (c2r0, c2r1, c2r2, c2r3)
	 *          col3 = (c3r0, c3r1, c3r2, c3r3)
	 *
	 *        So the first four parameters fill the first column, etc.
	 *
	 * @param c0r0 Row=0,Col=0
	 * @param c0r1 Row=1,Col=0
	 * @param c0r2 Row=2,Col=0
	 * @param c0r3 Row=3,Col=0
	 * @param c1r0 Row=0,Col=1
	 * @param c1r1 Row=1,Col=1
	 * @param c1r2 Row=2,Col=1
	 * @param c1r3 Row=3,Col=1
	 * @param c2r0 Row=0,Col=2
	 * @param c2r1 Row=1,Col=2
	 * @param c2r2 Row=2,Col=2
	 * @param c2r3 Row=3,Col=2
	 * @param c3r0 Row=0,Col=3
	 * @param c3r1 Row=1,Col=3
	 * @param c3r2 Row=2,Col=3
	 * @param c3r3 Row=3,Col=3
	 */
	inline Matrix4(float c0r0, float c0r1, float c0r2, float c0r3, float c1r0,
	               float c1r1, float c1r2, float c1r3, float c2r0, float c2r1,
	               float c2r2, float c2r3, float c3r0, float c3r1, float c3r2,
	               float c3r3) {
		m[0] = c0r0;
		m[1] = c0r1;
		m[2] = c0r2;
		m[3] = c0r3;
		m[4] = c1r0;
		m[5] = c1r1;
		m[6] = c1r2;
		m[7] = c1r3;
		m[8] = c2r0;
		m[9] = c2r1;
		m[10] = c2r2;
		m[11] = c2r3;
		m[12] = c3r0;
		m[13] = c3r1;
		m[14] = c3r2;
		m[15] = c3r3;
	}

	/**
	 * @brief Construct from a pointer to 16 floats (column-major).
	 * @param data Pointer to float array, 16 consecutive floats.
	 */
	inline explicit Matrix4(const float &data) {
		for (int i = 0; i < 16; i++) {
			m[i] = data;
		}
	}

	//----------------------------------------------------------------------------------------
	// Static Functions
	//----------------------------------------------------------------------------------------

	/**
	 * @brief Create and return an identity matrix.
	 */
	inline static Matrix4 Identity() {
		Matrix4 mat;
		mat.SetIdentity();
		return mat;
	}

	/**
	 * @brief Create and return a zero matrix.
	 */
	inline static Matrix4 Zero() {
		Matrix4 mat;
		for (int i = 0; i < 16; i++) {
			mat.m[i] = 0;
		}
		return mat;
	}

	/**
	 * @brief Create a 4x4 translation transform for 3D.
	 * @param x Translation in X
	 * @param y Translation in Y
	 * @param z Translation in Z
	 */
	inline static Matrix4 Translation(float x, float y, float z) {
		// Column-major layout for a standard 3D translation:
		//   [ 1  0  0  tx ]
		//   [ 0  1  0  ty ]
		//   [ 0  0  1  tz ]
		//   [ 0  0  0  1  ]
		//
		// col0 = (1, 0, 0, 0)
		// col1 = (0, 1, 0, 0)
		// col2 = (0, 0, 1, 0)
		// col3 = (tx,ty,tz,1)

		Matrix4 mat = Identity();
		mat.m[12] = x;
		mat.m[13] = y;
		mat.m[14] = z;
		return mat;
	}

	/**
	 * @brief Create a non-uniform scale transform.
	 * @param sx Scale factor on X
	 * @param sy Scale factor on Y
	 * @param sz Scale factor on Z
	 */
	inline static Matrix4 Scale(float sx, float sy, float sz) {
		//   [ sx  0   0   0 ]
		//   [ 0   sy  0   0 ]
		//   [ 0   0   sz  0 ]
		//   [ 0   0   0   1 ]
		Matrix4 mat = Identity();
		mat.m[0] = sx;
		mat.m[5] = sy;
		mat.m[10] = sz;
		return mat;
	}

	/**
	 * @brief Create a rotation matrix about the X axis.
	 * @param radians Rotation angle in radians.
	 */
	inline static Matrix4 RotationX(float radians) {
		// Rotate about X axis:
		//   [ 1    0      0    0 ]
		//   [ 0   cosθ   sinθ  0 ]
		//   [ 0  -sinθ   cosθ  0 ]
		//   [ 0    0      0    1 ]
		Matrix4 mat = Identity();
		float c = std::cos(radians);
		float s = std::sin(radians);
		mat.m[5] = c;
		mat.m[6] = s;
		mat.m[9] = -s;
		mat.m[10] = c;
		return mat;
	}

	/**
	 * @brief Create a rotation matrix about the Y axis.
	 * @param radians Rotation angle in radians.
	 */
	inline static Matrix4 RotationY(float radians) {
		// Rotate about Y axis:
		//   [ cosθ   0  -sinθ  0 ]
		//   [ 0      1    0    0 ]
		//   [ sinθ   0   cosθ  0 ]
		//   [ 0      0    0    1 ]
		Matrix4 mat = Identity();
		float c = std::cos(radians);
		float s = std::sin(radians);
		mat.m[0] = c;
		mat.m[2] = -s;
		mat.m[8] = s;
		mat.m[10] = c;
		return mat;
	}

	/**
	 * @brief Create a rotation matrix about the Z axis.
	 * @param radians Rotation angle in radians.
	 */
	inline static Matrix4 RotationZ(float radians) {
		// Rotate about Z axis:
		//   [ cosθ   sinθ  0  0 ]
		//   [ -sinθ  cosθ  0  0 ]
		//   [ 0       0    1  0 ]
		//   [ 0       0    0  1 ]
		Matrix4 mat = Identity();
		float c = std::cos(radians);
		float s = std::sin(radians);
		mat.m[0] = c;
		mat.m[1] = s;
		mat.m[4] = -s;
		mat.m[5] = c;
		return mat;
	}

	static Matrix4 Perspective(float fovRadians, float aspect, float zNear,
	                           float zFar);
	static Matrix4 Orthographic(float left, float right, float bottom,
	                            float top, float zNear, float zFar);
	static Matrix4 LookAt(const Vectors::Vector3 &eye,
	                      const Vectors::Vector3 &center,
	                      const Vectors::Vector3 &up);

	//----------------------------------------------------------------------------------------
	// Basic Accessors
	//----------------------------------------------------------------------------------------
	// Access Element by (row, column).
	inline float &operator()(int row, int col) { return m[col * 4 + row]; }
	inline const float &operator()(int row, int col) const {
		return m[col * 4 + row];
	}

	/**
	 * @brief Access element at index (0..15). Column-major usage.
	 * @param index The array index.
	 * @return Reference to the float at that index.
	 */
	inline float &operator[](int index) { return m[index]; }

	//----------------------------------------------------------------------------------------
	// Comparison
	//----------------------------------------------------------------------------------------

	/**
	 * @brief Const access element at index (0..15). Column-major usage.
	 */
	inline const float &operator[](int index) const { return m[index]; }

	/**
	 * @brief Equality check within a small epsilon for floating-point
	 * comparisons.
	 */
	inline bool operator==(const Matrix4 &rhs) const {
		const float EPS = 1e-6f;
		for (int i = 0; i < 16; ++i) {
			if (std::fabs(m[i] - rhs.m[i]) > EPS)
				return false;
		}
		return true;
	}

	/**
	 * @brief Inequality check.
	 */
	inline bool operator!=(const Matrix4 &rhs) const { return !(*this == rhs); }

	//----------------------------------------------------------------------------------------
	// Arithmetic
	//----------------------------------------------------------------------------------------
	/**
	 * @brief Matrix addition.
	 */
	inline Matrix4 operator+(const Matrix4 &rhs) const {
		Matrix4 result;
		for (int i = 0; i < 16; ++i)
			result.m[i] = m[i] + rhs.m[i];
		return result;
	}

	inline Matrix4 &operator+=(const Matrix4 &rhs) {
		for (int i = 0; i < 16; ++i)
			m[i] += rhs.m[i];
		return *this;
	}

	/**
	 * @brief Matrix subtraction.
	 */
	inline Matrix4 operator-(const Matrix4 &rhs) const {
		Matrix4 result;
		for (int i = 0; i < 16; ++i)
			result.m[i] = m[i] - rhs.m[i];
		return result;
	}

	inline Matrix4 &operator-=(const Matrix4 &rhs) {
		for (int i = 0; i < 16; ++i)
			m[i] -= rhs.m[i];
		return *this;
	}

	/**
	 * @brief Scalar multiplication.
	 */
	inline Matrix4 operator*(float s) const {
		Matrix4 result;
		for (int i = 0; i < 16; ++i)
			result.m[i] = m[i] * s;
		return result;
	}

	inline Matrix4 &operator*=(float s) {
		for (int i = 0; i < 16; ++i)
			m[i] *= s;
		return *this;
	}

	/**
	 * @brief Matrix multiplication (this * rhs).
	 */
	Matrix4 operator*(const Matrix4 &rhs) const;

	inline Matrix4 &operator*=(const Matrix4 &rhs) {
		*this = (*this) * rhs;
		return *this;
	}
	//----------------------------------------------------------------------------------------
	// Advanced
	//----------------------------------------------------------------------------------------

	/**
	 * @brief Get a transposed copy of this matrix.
	 * @return Transposed matrix.
	 */
	Matrix4 GetTransposed() const;

	/**
	 * @brief Transpose this matrix in place.
	 */
	inline void Transpose() { *this = GetTransposed(); }

	/**
	 * @brief Calculate and return the determinant of this matrix.
	 */
	float Determinant() const;

	/**
	 * @brief Invert this matrix in place (if possible).
	 * @return True if the matrix was inverted, false if singular (det too
	 * small).
	 */
	bool Invert();

	/**
	 * @brief Return the inverse of this matrix (if invertible).
	 * @param success Optional pointer to store if the inversion succeeded.
	 */
	Matrix4 GetInverse(bool *success = nullptr) const;
	//----------------------------------------------------------------------------------------
	// Utilities
	//----------------------------------------------------------------------------------------
	/**
	 * @brief Set this matrix to the identity matrix.
	 */
	inline void SetIdentity() {
		m[0] = 1.0f;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] = 1.0f;
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = 1.0f;
		m[11] = 0.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = 1.0f;
	}
	inline bool IsAffine() const {
		return (std::fabs(m[3]) < 1e-6f && std::fabs(m[7]) < 1e-6f &&
		        std::fabs(m[11]) < 1e-6f && std::fabs(m[15] - 1.0f) < 1e-6f);
	}

  private:
	static Matrix4 InvertAffine(const Matrix4 &mat);
	static Matrix4 InvertFullCofactorMethod(const Matrix4 &mat);
};
	
/**
 * @brief Allow scalar * Matrix4 from the left (e.g., 2.0f * mat).
 */
inline Matrix4 operator*(float s, const Matrix4 &mat) { return mat * s; }
} // namespace TinyMath3D::Matrices
