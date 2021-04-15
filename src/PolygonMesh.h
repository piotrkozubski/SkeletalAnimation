/*
 * PolygonMeshContainer.h
 *
 *  Created on: 22.07.2019
 *      Author: piotr.kozubski
 */

#ifndef POLYGONMESH_H
#define POLYGONMESH_H

#include <vector>
#include <stdint.h>
#include "Array.h"
#include "Types3d.h"

/**
 *
 */
template <uint32_t size> struct PolygonMesh
{
	std::vector<VertexPos> vertexPos;
	std::vector<Normal> vertexNormal;
	std::vector<VertexUV> vertexUV;
	std::vector<VertexBoneID> vertexBoneID;
	std::vector<VertexBoneWeight> vertexBoneWeight;
	std::vector< Face<size> > face;
	std::vector<Normal> faceNormal;

	/**
	 *
	 */
	void setVertexPos(const Array<VertexPos>& pVertexPos);

	/**
	 *
	 */
	void setVertexUV(const Array<VertexUV>& pVertexUV);

	/**
	 *
	 */
	void setFace(const Array< Face<3> >& pFace);

	/**
	 *
	 */
	void setFaceNormal(const Array<Normal>& pFaceNormal);
};

#endif /* POLYGONMESH_H */
