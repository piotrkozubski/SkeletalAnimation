/*
 * PolygonMesh.cpp
 *
 *  Created on: 22.07.2019
 *      Author: zokp
 */
#include "PolygonMesh.h"
#include <cstring>

template <uint32_t size>
void PolygonMesh<size>::setVertexPos(const Array<VertexPos>& pVertexPos)
{
	for (uint32_t i = 0; i < pVertexPos.mArraySize; ++i)
	{
		VertexPos vDst;
		memcpy(&vDst, &pVertexPos.mArray[i], sizeof(VertexPos));
		vertexPos.push_back(vDst);
	}
}

template <uint32_t size>
void PolygonMesh<size>::setVertexUV(const Array<VertexUV>& pVertexUV)
{
	for (uint32_t i = 0; i < pVertexUV.mArraySize; ++i)
	{
		VertexUV vDst;
		memcpy(&vDst, &pVertexUV.mArray[i], sizeof(VertexUV));
		vertexUV.push_back(vDst);
	}
}

template <uint32_t size>
void PolygonMesh<size>::setFace(const Array< Face<3> >& pFace)
{
	for (uint32_t i = 0; i < pFace.mArraySize; ++i)
	{
		Face<3> fDst;
		memcpy(&fDst, &pFace.mArray[i], sizeof(Face<3>));
		face.push_back(fDst);
	}
}

template <uint32_t size>
void PolygonMesh<size>::setFaceNormal(const Array<Normal>& pFaceNormal)
{
	for (uint32_t i = 0; i < pFaceNormal.mArraySize; ++i)
	{
		Normal nDst;
		memcpy(&nDst, &pFaceNormal.mArray[i], sizeof(Normal));
		faceNormal.push_back(nDst);
	}
}

static void temporatyFunc(void)
{
	PolygonMesh<1> p1;
	PolygonMesh<2> p2;
	PolygonMesh<3> p3;
	Array<VertexPos> a(1);
	Array<VertexUV> b(1);
	Array< Face<3> > c(1);
	Array<Normal> d(1);
	p3.setVertexPos(a);
	p3.setVertexUV(b);
	p3.setFace(c);
	p3.setFaceNormal(d);
}
