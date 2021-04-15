/*
 * Pmesh.h
 *
 *  Created on: 14.06.2019
 *      Author: piotr.kozubski
 */

#ifndef PMESH_H_
#define PMESH_H_

#include <auto_ptr.h>

#include "BoneKeyFrames.h"
#include "OGLPolygonMesh.h"

class OGLPolygonMesh;
class BoneKeyFrames;

/**
 * PMESH file format class
 * Pmesh object can return references to sub-objects:
 * OGLPolygonMesh - responsible for loading vertex attributes and triangles
 * 					into OpenGL (buffers on GPU)
 * BonesFrame     - if defines in PMESH file, bones are collected in
 * 					BonesFrames object.
 * 					BonesFrames can return bones data for for given frame.
 */
class Pmesh
{
public:

	/**
	 *  Static member function responsible for Pmesh object creation.
	 *  Opens and parses .pmesh file. Data is stored in mPolygonMesh and
	 *  mBonesFrame objects.
	 *
	 *  @param      Name of the file containing polygon mesh data.
	 *
	 *  @return     pointer to OGLPolyonMesh object
	 */
	static Pmesh* createFromFile(const char* pmeshFileName);

	/**
	 * Returns pointer to OGLPolygonMesh object
	 *
	 * @param 		no params
	 * @return		pointer to OGLPolygonMesh object
	 */
	OGLPolygonMesh* getPolygonMesh(void);

	/**
	 * Returns pointer to BonesFrame object
	 *
	 * @param 		no params
	 * @return		pointer to BonesFrame object
	 */
	BoneKeyFrames* getBonesFrame(void);

	/**
	 *  Destructor
	 */
	~Pmesh() {}

	/**
	 * typedef of smart pointer to Pmesh
	 */
	typedef std::auto_ptr<Pmesh> Ptr;

private:
	OGLPolygonMesh::Ptr		mPolygonMesh;
	BoneKeyFrames::Ptr		mBoneKeyFrames;

	/**
	 * Private constructor
	 *
	 * @param pPolygonMesh		pointer to OGLPolygonMesh object
	 * @param pBonesFrame		pointer to BonesFrame object
	 */
	Pmesh(OGLPolygonMesh *pPolygonMesh, BoneKeyFrames *pBoneFrames);
};

inline Pmesh::Pmesh(OGLPolygonMesh *pPolygonMesh, BoneKeyFrames *pBonesFrame) :
		mPolygonMesh(pPolygonMesh),
		mBoneKeyFrames(pBonesFrame)
{ }

inline OGLPolygonMesh* Pmesh::getPolygonMesh(void)
{
	return mPolygonMesh.get();
}

inline BoneKeyFrames* Pmesh::getBonesFrame(void)
{
	return mBoneKeyFrames.get();
}

#endif /* PMESH_H_ */
