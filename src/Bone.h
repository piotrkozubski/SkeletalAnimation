/*
 * Bone.h
 *
 *  Created on: 23.06.2019
 *      Author: zokp
 */

#ifndef BONE_H_
#define BONE_H_

#include <auto_ptr.h>
#include "Logger.h"
#include "Matrix.h"
#include "Array.h"
#include "PolygonMesh.h"

/**
 * Bone class holds transformation matrix for one bone,
 *  and poiter to next bone.
 */
class Bone
{
public:

	static const uint16_t NO_PARENT_IDX = 0xFF;

	/**
	 * typedef of smart pointer to Bone
	 */
	typedef std::auto_ptr<Bone>	Ptr;

	/**
	 * Constructor
	 *
	 * @param m		transformation matrix for bone
	 */
	Bone(const Matrix4x4f pMatrix, BoneID pParentIdx);

	/**
	 * Destructor
	 */
	~Bone() {}

	/**
	 * Returns mParentIdx
	 *
	 */
	uint16_t getParentIdx(void) { return mParentIdx; }

	/**
	 * mChildren setter
	 *
	 * @param pNext		pointer array of Bone pointers
	 */
	void setChildren(Array<Bone*>* bones);

	/**
	 * Transforms bone matrix and its children
	 * Modifies mFrameMatrix
	 *
	 * @param m			rotation matrix
	 * @param mRelative	relative matrix, from which rotation is done
	 */
	void transform(const Matrix4x4f& m, const Bone* b);

	/**
	 * const transformation matrix
	 */
	const Matrix4x4f	mPoseMatrix;

	Matrix4x4f			mFramePoseMatrix;

	Matrix4x4f			mModificationMatrix;

	/**
	 * const inverse transformation matrix
	 */
	Matrix4x4f			mInverseMatrix;

private:
	BoneID	 			mParentIdx;
	Array<Bone*>::Ptr 	mChildren;
};

#endif /* BONE_H_ */
