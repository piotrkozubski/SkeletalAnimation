/*
 * Matrix.h
 *
 *  Created on: 07.06.2017
 *      Author: Piotr Kozubski
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <string>

#define USE_GLM

#ifdef USE_GLM
#include <glm/mat4x4.hpp>
typedef glm::mat4 Matrix4x4f;
typedef float Quat[4];
#else
typedef float Matrix4x4f[16];
#endif

/**
 * Matrix name-space
 *
 * Matrix name-space should contain non-member functions which implements
 * operations for matrix.
 *
 */
namespace Matrix
{
	/**
	 * Returns pointer to memory address of given matrix structure
	 */
	void* getMatrix4x4Pointer(Matrix4x4f& m);

	/**
	 *  Sets identity matrix to 4x4f matrix.
	 *
	 *	@param m	matrix that will be set to identity
	 *
	 */
	void setIdentityMatrix4x4f(Matrix4x4f& m);

	/**
	 *  Sets perspective projection matrix to 4x4f matrix.
	 *
	 *	@param m	reference to matrix that will have perspective projection
	 *	@param l	left (x) border of view rectangle
	 *	@param r	right (x) border of view rectangle
	 *	@param b	bottom (y) border of view rectangle
	 *	@param t	top (y) border of view rectangle
	 *	@param n	near (z) border of view rectangle
	 *	@param f	far (z) border of view rectangle
	 *
	 */
	void setPerspectiveMatrix4x4f(Matrix4x4f& m,
								  float l, float r,
								  float b, float t,
								  float n, float f);

	/**
	 *  Sets perspective projection matrix to 4x4f matrix.
	 *
	 *  @param angle     	angle of view
	 *  @param aspectRatio 	proportion of viewport's width to height
	 *	@param n	  		near (z) border of view rectangle
	 *	@param f	  		far (z) border of view rectangle
	 *
	 */
	void setPerspectiveMatrix4x4f(Matrix4x4f& m,
			  double angle, double aspectRatio,
			  float n, float f);

	/**
	 *  Sets orthogonal matrix to 4x4f matrix.
	 *
	 *	@param m	reference to matrix that will have orthogonal projection
	 *	@param l	left (x) border of view rectangle
	 *	@param r	right (x) border of view rectangle
	 *	@param b	bottom (y) border of view rectangle
	 *	@param t	top (y) border of view rectangle
	 *	@param n	near (z) border of view rectangle
	 *	@param f	far (z) border of view rectangle
	 *
	 */
	void setOrthoMatrix4x4f(Matrix4x4f& m,
						    float l, float r,
							float b, float t,
							float n, float f);

	/**
	 *  Sets orthogonal matrix to 4x4f matrix.
	 *
	 *	@param m			reference to matrix that will have orthogonal projection
	 *  @param aspectRatio 	proportion of viewport's width to height
	 *	@param n			near (z) border of view rectangle
	 *	@param f			far (z) border of view rectangle
	 *
	 */
	void setOrthoMatrix4x4f(Matrix4x4f& m, float aspectRatio, float n, float f);

	/**
	 * Translates 4x4 float matrix by given values
	 *
	 * @param xTran		translation on x axis
	 * @param yTran		translation on y axis
	 * @param zTran		translation on z axis
	 *
	 */
	void translateMatrix4x4f(Matrix4x4f& m,
							 float xTrans, float yTrans, float zTrans);

	/**
	 * Multiplies matrices a x b
	 *
	 * @param m		matrix multiplication result
	 * @param a		first matrix
	 * @param b		second matrix
	 */
	void mulMatrix4x4f(Matrix4x4f& m, const Matrix4x4f& a, const Matrix4x4f& b);

#ifndef USE_GLM
	/**
	 * Operator variant of mulMatrix4x4f
	 */
	const Matrix4x4f operator*(const Matrix4x4f& lhs, const Matrix4x4f& rhs);
#endif

	enum rotFlag
	{
		ROT_X_FLAG = 1,
		ROT_Y_FLAG = 2,
		ROT_Z_FLAG = 4
	};

	/**
	 * Rotates matrices by given angle
	 *
	 * @param m		matrix multiplication result
	 * @param angle rotation angle
	 * @param rotFlag rotation flag, see rotFlag enumerator
	*/
	Matrix4x4f& rotMatrix4x4f(Matrix4x4f& m, float angle, rotFlag rotFlag);

	/**
	 * Inverses matrix
	 *
	 * @param result	result of inversion
	 * @param m			matrix for inversion
	*/
	Matrix4x4f& inverseMatrix4x4f(Matrix4x4f& result, const Matrix4x4f& m);

	/**
	 * Converts quaternion to 4x4 matrix
	 */
	Matrix4x4f& quatToMatrix4x4f(Matrix4x4f& result, const Quat& q);

	/**
	 * Converts matrix to string
	 *
	 * @return string
	 */
	std::string toString(const Matrix4x4f& m);

	/**
	 * Interpolates two quaternions
	 */
	Matrix4x4f& quatInterpolateSlerp(Matrix4x4f& result,
			const Quat& firstQuat, const Quat& secondQuat, float factor);
}

#endif /* MATRIX_H_ */
