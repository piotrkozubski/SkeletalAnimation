/*
 * Bone.cpp
 *
 *  Created on: 23.06.2019
 *      Author: zokp
 */

#include "Bone.h"
#include "Logger.h"
#include "Matrix.h"

Bone::Bone(const Matrix4x4f pMatrix, BoneID pParentIdx) :
		mPoseMatrix(pMatrix),
		mFramePoseMatrix(mPoseMatrix),
		mModificationMatrix(1.0),
		mParentIdx(pParentIdx),
		mChildren(NULL)
{
	LOG_DEB("Bone created, parent index: " << mParentIdx <<
			" matrix: " << Matrix::toString(mPoseMatrix))

	Matrix::inverseMatrix4x4f(mInverseMatrix, mPoseMatrix);
}

void Bone::setChildren(Array<Bone*>* bones)
{
	mChildren.reset(bones);
}

void Bone::transform(const Matrix4x4f& m, const Bone* b)
{
	//LOG_INF(Matrix::toString(m));
	if (b == this)
	{
		Matrix::mulMatrix4x4f(mModificationMatrix, m, mModificationMatrix);
	}
	else
	{
//LOG_INF("1: " << b << ", " << mFramePoseMatrix[0][0] << ", " << mFramePoseMatrix[1][0] << ", " << mFramePoseMatrix[2][0] << ", " << mFramePoseMatrix[3][0]);
//LOG_INF("1: " << b << ", " << mFramePoseMatrix[0][1] << ", " << mFramePoseMatrix[1][1] << ", " << mFramePoseMatrix[2][1] << ", " << mFramePoseMatrix[3][1]);
//LOG_INF("1: " << b << ", " << mFramePoseMatrix[0][2] << ", " << mFramePoseMatrix[1][2] << ", " << mFramePoseMatrix[2][2] << ", " << mFramePoseMatrix[3][2]);
//LOG_INF("1: " << b << ", " << mFramePoseMatrix[0][3] << ", " << mFramePoseMatrix[1][3] << ", " << mFramePoseMatrix[2][3] << ", " << mFramePoseMatrix[3][3]);

		Matrix4x4f temp = mFramePoseMatrix;
		temp[3][0] -= b->mPoseMatrix[3][0];
		temp[3][1] -= b->mPoseMatrix[3][1];
		temp[3][2] -= b->mPoseMatrix[3][2];

		float diffX = temp[3][0];
		float diffY = temp[3][1];
		float diffZ = temp[3][2];

//LOG_INF("2: " << b << ", " << temp[0][0] << ", " << temp[1][0] << ", " << temp[2][0] << ", " << temp[3][0]);
//LOG_INF("2: " << b << ", " << temp[0][1] << ", " << temp[1][1] << ", " << temp[2][1] << ", " << temp[3][1]);
//LOG_INF("2: " << b << ", " << temp[0][2] << ", " << temp[1][2] << ", " << temp[2][2] << ", " << temp[3][2]);
//LOG_INF("2: " << b << ", " << temp[0][3] << ", " << temp[1][3] << ", " << temp[2][3] << ", " << temp[3][3]);

		Matrix::mulMatrix4x4f(temp, temp, m);
//		Matrix::mulMatrix4x4f(mModificationMatrix, m, mModificationMatrix);
		temp[3][0] -= diffX;
		temp[3][1] -= diffY;
		temp[3][2] -= diffZ;

//LOG_INF("3: " << b << ", " << temp[0][0] << ", " << temp[1][0] << ", " << temp[2][0] << ", " << temp[3][0]);
//LOG_INF("3: " << b << ", " << temp[0][1] << ", " << temp[1][1] << ", " << temp[2][1] << ", " << temp[3][1]);
//LOG_INF("3: " << b << ", " << temp[0][2] << ", " << temp[1][2] << ", " << temp[2][2] << ", " << temp[3][2]);
//LOG_INF("3: " << b << ", " << temp[0][3] << ", " << temp[1][3] << ", " << temp[2][3] << ", " << temp[3][3]);

//		temp[3][0] += b->mPoseMatrix[3][0];
//		temp[3][1] += b->mPoseMatrix[3][1];
//		temp[3][2] += b->mPoseMatrix[3][2];

		Matrix::mulMatrix4x4f(mModificationMatrix, mModificationMatrix, temp);

//LOG_INF("4: " << b << ", " << mModificationMatrix[0][0] << ", " << mModificationMatrix[1][0] << ", " << mModificationMatrix[2][0] << ", " << mModificationMatrix[3][0]);
//LOG_INF("4: " << b << ", " << mModificationMatrix[0][1] << ", " << mModificationMatrix[1][1] << ", " << mModificationMatrix[2][1] << ", " << mModificationMatrix[3][1]);
//LOG_INF("4: " << b << ", " << mModificationMatrix[0][2] << ", " << mModificationMatrix[1][2] << ", " << mModificationMatrix[2][2] << ", " << mModificationMatrix[3][2]);
//LOG_INF("4: " << b << ", " << mModificationMatrix[0][3] << ", " << mModificationMatrix[1][3] << ", " << mModificationMatrix[2][3] << ", " << mModificationMatrix[3][3]);

//		Matrix4x4f invTemp(1.0);
//		Matrix::inverseMatrix4x4f(invTemp, mModificationMatrix);
//		Matrix::mulMatrix4x4f(mInverseMatrix, invTemp, mInverseMatrix);
		//Matrix::inverseMatrix4x4f(mInverseMatrix, mFramePoseMatrix);
	}

	for (BoneID id = 0; id < mChildren->mArraySize; ++id)
	{
		mChildren->mArray[id]->transform(m, b);
	}
}
