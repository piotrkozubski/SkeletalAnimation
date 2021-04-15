/*
 * BSPTree.cpp

 *
 *  Created on: 19.07.2019
 *      Author: piotr.kozubski
 */

#include "BSPTree.h"
#include "Logger.h"
#include "OGLPolygonMesh.h"
#include "Pmap.h"
#include "Math3d.h"

static void split(PolygonMeshList& pMeshList, PolygonMesh<3>& pMesh);
static bool findOpposedPlane(PolygonMesh<3>& pMesh, uint32_t& triangleIndex);

BSPTree* BSPTree::create(const Pmap& pPmap)
{
	LOG_INF("Creating BSPTree from Pmap object");

	PolygonMeshList meshList;
	PolygonMesh<3> mesh;

	mesh.setVertexPos(pPmap.getVertexPosList());
	mesh.setVertexUV(pPmap.getVertexUVList());
	mesh.setFace(pPmap.getFaceList());
	mesh.setFaceNormal(pPmap.getFaceNormalList());

	split(meshList, mesh);

	return new BSPTree(meshList);
}

BSPTree::BSPTree(PolygonMeshList pMeshList):
		mMeshList(pMeshList)
{
	LOG_INF("BSPTree constructor");
	LOG_ASSERT(mMeshList.size(), "mesh list empty");
}

BSPTree::~BSPTree()
{
	LOG_INF("BSPTree destructor");
}

static void split(PolygonMeshList& pMeshList, PolygonMesh<3>& pMesh)
{
	uint32_t triangleIndex = 0;
	if (!findOpposedPlane(pMesh, triangleIndex))
	{
		OGLPolygonMesh* pm =
				OGLPolygonMesh::create<OGLPolygonMesh::TRIANGLE>(pMesh, 0);

		pMeshList.push_back(pm);
		return;
	}
	else
	{
		LOG_INF("found: " << triangleIndex);
	}
}

static bool findOpposedPlane(PolygonMesh<3>& pMesh, uint32_t& triangleIndex)
{
	return false;
	std::vector< Face<3> >::iterator it = pMesh.face.begin();
	uint32_t idx = 0;

	while (it != pMesh.face.end())
	{
		idx = std::distance(pMesh.face.begin(), it);
		VertexPos t1;
		t1 = pMesh.vertexPos[(*it).f[0]];
		Normal n = pMesh.faceNormal[idx];

		LOG_INF("t1 face: " << (*it).f[0] << ", pos: " << t1.pos[0] << ", " << t1.pos[1] << ", " << t1.pos[2]
                << ", normal: " << n.n[0] << ", " << n.n[1] << ", " << n.n[2]);


		std::vector< Face<3> >::iterator it2 = it + 1;
		while (it2 != pMesh.face.end())
		{
			for (uint32_t vIdx = 0; vIdx < 3; ++vIdx)
			{
				VertexPos t2 = pMesh.vertexPos[(*it2).f[vIdx]];

				LOG_INF("t2 face: " << (*it2).f[vIdx] << ", pos: " << t2.pos[0] << ", " << t2.pos[1] << ", " << t2.pos[2]);

				if (Math3d::isVertexBehind(t1, t2, n))
				{
					triangleIndex = idx;
					return true;
				}
			}

			it2++;
		}
		it++;
	}

	return false;
}
