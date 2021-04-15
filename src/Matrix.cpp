/*
 * Matrix.cpp
 *
 *  Created on: 15.07.2017
 *      Author: Piotr Kozubski
 */

#include "Matrix.h"
#include <cmath>

#ifdef USE_GLM
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Logger.h"
#endif

void* Matrix::getMatrix4x4Pointer(Matrix4x4f& m)
{
#ifdef USE_GLM
	return glm::value_ptr(m);
#else
	return &m;
#endif
}
void Matrix::setIdentityMatrix4x4f(Matrix4x4f& m)
{
#ifdef USE_GLM
	m[0][0] = 1.0f; m[1][0] = 0.0f; m[2][0] = 0.0f; m[3][0] = 0.0f;
	m[0][1] = 0.0f; m[1][1] = 1.0f; m[2][1] = 0.0f; m[3][1] = 0.0f;
	m[0][2] = 0.0f; m[1][2] = 0.0f; m[2][2] = 1.0f; m[3][2] = 0.0f;
	m[0][3] = 0.0f; m[1][3] = 0.0f; m[2][3] = 0.0f; m[3][3] = 1.0f;
#else
	m[0] =  1.0; m[1] =  0.0; m[2] =  0.0; m[3] =  0.0;
	m[4] =  0.0; m[5] =  1.0; m[6] =  0.0; m[7] =  0.0;
	m[8] =  0.0; m[9] =  0.0; m[10] = 1.0; m[11] = 0.0;
	m[12] = 0.0; m[13] = 0.0; m[14] = 0.0; m[15] = 1.0;
#endif
}



void Matrix::setPerspectiveMatrix4x4f(Matrix4x4f& m,
		  	  	  	  	  	  	  	  float l, float r,
									  float b, float t,
									  float n, float f)
{
#ifdef USE_GLM
	LOG_ASSERT(0, "not implemented for GLM");
#else
	setIdentityMatrix4x4f(m);
	m[0] = (2.0 * n) / (r - l);
	m[1] = 0.0;
	m[2] = (r + l) / (r - l);
	m[3] = 0.0;

	m[4] = 0.0;
	m[5] = (2.0 * n) / (t - b);
	m[6] = (t + b) / (t - b);
	m[7] = 0.0;

	m[8] = 0.0;
	m[9] = 0.0;
	m[10] = (-(f + n)) / (f - n);
	m[11] = (-(2.0 * f * n) / (f - n));

	m[12] =  0.0;
	m[13] =  0.0;
	m[14] = -1.0;
	m[15] =  0.0;
#endif
}

void Matrix::setPerspectiveMatrix4x4f(Matrix4x4f& m,
								  double angle, double aspectRatio,
								  float n, float f)
{
#ifdef USE_GLM
	m = glm::perspective<double>(angle, aspectRatio, n, f);
#else
	const double DEG2RAD = 3.14159265 / 180;
//	double tanRad = tan(angle / 2.0 * DEG2RAD);
//	float height = n * tanRad;
//	float width = height * aspectRatio;
//	setPerspectiveMatrix4x4f(m, -width, width, -height, height, n, f);

#if 1
    const float ar = aspectRatio;
    const float zNear = n;
    const float zFar = f;
    const float zRange = zNear - zFar;
    const float tanHalfFOV = tan((angle / 2.0) * DEG2RAD);

    m[0] = 1.0f / (tanHalfFOV * ar);
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;

    m[4] = 0.0f;
    m[5] = 1.0f / tanHalfFOV;
    m[6] = 0.0f;
    m[7] = 0.0f;

    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = (-zNear - zFar) / zRange;
    m[11] = 2.0f * zFar * zNear / zRange;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 1.0f;
    m[15] = 0.0f;
#endif
#endif
}

void Matrix::setOrthoMatrix4x4f(Matrix4x4f& m,
	  	  	  	  	  	  	    float l, float r,
								float b, float t,
								float n, float f)
{
#ifdef USE_GLM
	m = glm::ortho(l, r, b, t, n, f);
#else
	setIdentityMatrix4x4f(m);
	m[0] = 2.0 / (r - l);
	m[4] = 0.0;
	m[8] = 0.0;
	m[12] = -((r + l) / (r - l));

	m[1] = 0.0;
	m[5] = 2.0 / (t - b);
	m[9] = 0.0;
	m[13] = -((t + b) / (t - b));

	m[2] = 0.0;
	m[6] = 0.0;
	m[10] = -2.0 / (f - n);
	m[14] = -((f + n) / (f - n));

	m[3] = 0.0;
	m[7] = 0.0;
	m[11] = 0.0;
	m[15] = 1.0;
#endif
}

void Matrix::setOrthoMatrix4x4f(Matrix4x4f& m, float aspectRatio, float n, float f)
{
	float width = 1.0;
	float height = width / aspectRatio;
#ifdef USE_GLM
	m = glm::ortho(-width, width, -height, height, n, f);
#else
	setOrthoMatrix4x4f(m, -width, width, -height, height, n, f);
#endif
}

/*
 * 	mat4 modelTrans = mat4(
		1.0, 	   0.0, 	  0.0, 		 vTrans[0],
		0.0, 	   1.0, 	  0.0, 		 vTrans[1],
		0.0, 	   0.0, 	  1.0, 		 vTrans[2],
		0.0, 	   0.0, 	  0.0, 		 1.0
	);
	mat4 modelRotX = mat4(
		1.0, 0.0, 	  			0.0, 		      0.0,
		0.0, cos(vRot[0]),		-sin(vRot[0]), 	  0.0,
		0.0, sin(vRot[0]),		cos(vRot[0]), 	  0.0,
		0.0, 0.0, 	  			0.0, 		 	  1.0
	);
	mat4 modelRotY = mat4(
		cos(vRot[1]), 	   0.0,		sin(vRot[1]), 		 0.0,
		0.0,			   1.0, 	0.0, 		 		 0.0,
		-sin(vRot[1]), 	   0.0, 	cos(vRot[1]), 	  	 0.0,
		0.0, 	   		   0.0, 	0.0,         		 1.0
	);
	mat4 modelRotZ = mat4(
		cos(vRot[2]), 	   -sin(vRot[2]), 	  0.0, 		 0.0,
		sin(vRot[2]), 	   cos(vRot[2]), 	  0.0, 		 0.0,
		0.0, 	   					0.0, 	  1.0, 		 0.0,
		0.0, 	   					0.0, 	  0.0, 		 1.0
	);
	mat4 perspProj = mat4(
		(2.0 * 1.0) / (1 + 1),   0.0, 					(1 - 1) / (1 + 1), 				0.0,
		0.0, 				  	 (2.0 * 1.0) / (1 + 1), (1 - 1) / (1 + 1), 				0.0,
		0.0, 				  	 0.0, 					(-(10.0 + 1.0))/(10.0 - 1.0), 	(-(2.0 * 10.0 * 1.0)/(10.0 - 1.0)),
		0.0, 				  	 0.0, 					-1.0, 				0.0
	);
 *
 *
 */

void Matrix::translateMatrix4x4f(Matrix4x4f& m,
								 float xTrans, float yTrans, float zTrans)
{
#ifdef USE_GLM
	glm::vec3 vec3(xTrans, yTrans, zTrans);
	glm::mat4 transMat = glm::translate(m, vec3);
	m = transMat;
//	m[3][0] += xTrans;
//	m[3][1] += yTrans;
//	m[3][2] += zTrans;
#else
	m[12] += xTrans;
	m[13] += yTrans;
	m[14] += zTrans;
#endif
}

void Matrix::mulMatrix4x4f(Matrix4x4f& m, const Matrix4x4f& a, const Matrix4x4f& b)
{
#ifdef USE_GLM
	m = b * a;
#else
	m[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
	m[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
	m[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
	m[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

	m[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
	m[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
	m[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
	m[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

	m[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
	m[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
	m[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
	m[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

	m[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
	m[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
	m[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
	m[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
#endif
}

Matrix4x4f& Matrix::rotMatrix4x4f(Matrix4x4f& m, float angle, rotFlag rotFlag)
{
#ifdef USE_GLM
	float xRot = (rotFlag & ROT_X_FLAG) ? 1.0 : 0.0;
	float yRot = (rotFlag & ROT_Y_FLAG) ? 1.0 : 0.0;
	float zRot = (rotFlag & ROT_Z_FLAG) ? 1.0 : 0.0;
	glm::vec3 v(xRot, yRot, zRot);
	m = glm::rotate(m, angle, v);
	return m;
#else
#endif
}

Matrix4x4f& Matrix::inverseMatrix4x4f(Matrix4x4f& result, const Matrix4x4f& m)
{
#ifdef USE_GLM
	result = glm::inverse(m);
	return result;
#else
#endif
}

Matrix4x4f& Matrix::quatToMatrix4x4f(Matrix4x4f& result, const Quat& q)
{
//#ifdef USE_GLM
#if 1
	glm::quat glmQ(q[3], q[0], q[1], q[2]);
	result = glm::toMat4(glmQ);
	return result;
#else
	float qx = q[0];
	float qy = q[1];
	float qz = q[2];
	float qw = q[3];
	result[0][0] = 1 - (2*qy*qy - (2*qz*qz)); result[0][1] = (2*qx*qy) - (2*qw*qz); result[0][2] = (2*qx*qz)+(2*qw*qy); 	result[0][3] = 0;
	result[1][0] = (2*qx*qy)+(2*qw*qz); 	  result[1][1] = 1-(2*qx*qx)-(2*qz*qz); result[1][2] = (2*qy*qz)-(2*qw*qx); 	result[1][3] = 0;
	result[2][0] = (2*qx*qz)-(2*qw*qy); 	  result[2][1] = (2*qy*qz)+(2*qw*qx); 	result[2][2] = 1-(2*qx*qx)-(2*qy*qy); 	result[2][3] = 0;
	result[3][0] = 0; 						  result[3][1] = 0; 					result[3][2] = 0; 						result[3][3] = 1.0;
//	result[0][0] = 1 - (2*qy*qy - (2*qz*qz)); result[0][1] = (2*qx*qy) - (2*qw*qz); result[0][2] = (2*qx*qz)+(2*qw*qy); 	result[0][3] = 0;
//	result[1][0] = (2*qx*qz)-(2*qw*qy); 	  result[1][1] = (2*qy*qz)+(2*qw*qx); 	result[1][2] = 1-(2*qx*qx)-(2*qy*qy); 	result[1][3] = 0;
//	result[2][0] = (2*qx*qy)+(2*qw*qz); 	  result[2][1] = 1-(2*qx*qx)-(2*qz*qz); result[2][2] = (2*qy*qz)-(2*qw*qx); 	result[2][3] = 0;
//	result[3][0] = 0; 						  result[3][1] = 0; 					result[3][2] = 0; 						result[3][3] = 1.0;
#endif
}

std::string Matrix::toString(const Matrix4x4f& m)
{
	return glm::to_string(m);
}

Matrix4x4f& Matrix::quatInterpolateSlerp(Matrix4x4f& result,
			const Quat& firstQuat, const Quat& secondQuat, float factor)
{
	glm::quat q1(firstQuat[3], firstQuat[0], firstQuat[1], firstQuat[2]);
	glm::quat q2(secondQuat[3], secondQuat[0], secondQuat[1], secondQuat[2]);
	result = glm::toMat4(glm::slerp(q1, q2, factor));

	return result;
}
