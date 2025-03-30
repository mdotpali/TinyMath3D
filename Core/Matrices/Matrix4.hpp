//
//  Matrix4.hpp
//  TinyMath3D
//
//  Created by Mostafa on 27.03.25.
//

#include <Core/Vectors/Vector3.hpp>
#include <cmath>

namespace TinyMath3D::Matrices {

// Column-Major  4x4 Matrix
// Memory layout (column-major), matching Apple/Metal's conventions:
//
//   m[ 0]  m[ 4]  m[ 8]  m[12]
//   m[ 1]  m[ 5]  m[ 9]  m[13]
//   m[ 2]  m[ 6]  m[10]  m[14]
//   m[ 3]  m[ 7]  m[11]  m[15]
//
// Columns are stored contiguously. For a transform M, the translation
// is typically in m[12], m[13], m[14], and the bottom row is in m[3], m[7],
// m[11], m[15].
class Matrix4 {

  public:
	// column-major array of 16 floats
	float m[16];

	//----------------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------------
	// Default Constructor
	inline Matrix4() { SetIdentity(); }

	// Inline Constructor from explicit float values (column major)
	inline Matrix4(float m00, float m10, float m20, float m30, float m01,
	               float m11, float m21, float m31, float m02, float m12,
	               float m22, float m32, float m03, float m13, float m23,
	               float m33)

	{
		m[0] = m00;
		m[1] = m01;
		m[2] = m02;
		m[3] = m03;
		m[4] = m10;
		m[5] = m11;
		m[6] = m12;
		m[7] = m13;
		m[8] = m20;
		m[9] = m21;
		m[10] = m22;
		m[11] = m23;
		m[12] = m30;
		m[13] = m31;
		m[14] = m32;
		m[15] = m33;
	}

	// Inline Constructor from a float pointer
	inline explicit Matrix4(const float *data) {
		for (int i = 0; i++; i < 16) {
			m[i] = data;
		}
	}

	//----------------------------------------------------------------------------------------
	// Static Functions
	//----------------------------------------------------------------------------------------
	// Returns Identity Matrix
	inline static Matrix4 Identity() {
		Matrix4 mat;
		mat.SetIdentity();
		return mat;
	}

	// Returns Matrix with all Elments of Zero
	inline static Matrix4 Zero() {
		Matrix4 mat;
		for (int i = 0; i < 16; i++) {
			m[i] = 0;
		}
		return mat;
	}

	// Returns Translation Matrix
	inline static Matrix4 Translation(float x, float y, float z) {
		Matrix4 mat = Identity();
		mat.m[12] = x;
		mat.m[13] = y;
		mat.m[14] = z;
		return mat;
	}

	// Returns Scale Matrix
	inline static Matrix4 Scale(float sx, float sy, float sz) {
		Matrix4 mat = Identity();
		mat.m[0] = sx;
		mat.m[5] = sy;
		mat.m[10] = sz;
	}

	// Returns Rotation Matrix for X
	inline static Matrix4 RotationX(float radians) {
		Matrix4 mat = Identity();
		float c = std::cos(radians);
		float s = std::sin(radians);
		mat.m[5] = c;
		mat.m[6] = s;
		mat.m[9] = -s;
		mat.m[10] = c;
		return mat;
	}

	// Returns Rotation Matrix for Y
	inline static Matrix4 RotationY(float radians) {
		Matrix4 mat = Identity();
		float c = std::cos(radians);
		float s = std::sin(radians);
		mat.m[0] = c;
		mat.m[2] = -s;
		mat.m[8] = s;
		mat.m[10] = c;
		return mat;
	}

	// Returns Rotation Matrix for Z
	inline static Matrix4 RotationZ(float radians) {
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
	static Matrix4 LookAt(const Vector3 &eye, const Vector3 &center,
	                      const Vector3 &up);

	//----------------------------------------------------------------------------------------
	// Basic Accessors
	//----------------------------------------------------------------------------------------
	// Access Element by (row, column).
	inline float &operator()(int row, int col) { return m[col * 4 + row]; }
	inline const float &operator()(int row, int col) {
		return m[col * 4 + row];
	}

	// Direct Access array-style
	inline float &operator[](int index) { return m[index]; }
	inline const float &operator[](int index) const { return m[index]; }

	//----------------------------------------------------------------------------------------
	// Multiplications
	//----------------------------------------------------------------------------------------
	// Matrix4 * Matrix4
	Matrix4 operator*(const Matrix4& rhs) const;
	Matrix4 operator*=(const Matrix4& rhs);
	
	//----------------------------------------------------------------------------------------
	// Transpose, Inversion
	//----------------------------------------------------------------------------------------
	inline Matrix4 Transposed() const;
	inline void Transpose(){
		*this = Transposed();
	}
	
	static Matrix4 Invert(const Matrix4& mat);
	void Invert();
	//----------------------------------------------------------------------------------------
	// Utilities
	//----------------------------------------------------------------------------------------
	// Set Identity Matrix
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
};
} // namespace TinyMath3D::Matrices
