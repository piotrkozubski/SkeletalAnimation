/*
 * FloorMesh.cpp
 *
 *  Created on: 13.05.2019
 *      Author: piotr.kozubski
 */
#include "GridMesh.h"

#include <cstddef>
#include <GL/glcorearb.h>

#include "OGLPolygonMesh.h"
#include "OGLProgramAttrib.h"
#include "Logger.h"

OGLPolygonMesh* GridMesh::create(void)
{
	const uint8_t X_NUM = 50;
	PolygonMesh<2> polygonMesh;
	polygonMesh.vertexPos.resize(X_NUM * 2);
	polygonMesh.face.resize(X_NUM);

	for (int x = 0; x < X_NUM; ++x)
	{
		const uint8_t Q = (X_NUM * 1.0f) / 4;
		if (x < (X_NUM / 2))
		{
			polygonMesh.vertexPos[x * 2 + 0].pos[0] = x * 1.0f - Q;
			polygonMesh.vertexPos[x * 2 + 0].pos[1] = 0.0f;
			polygonMesh.vertexPos[x * 2 + 0].pos[2] = -Q;

			polygonMesh.vertexPos[x * 2 + 1].pos[0] = x * 1.0f - Q;
			polygonMesh.vertexPos[x * 2 + 1].pos[1] = 0.0f;
			polygonMesh.vertexPos[x * 2 + 1].pos[2] = Q;

			polygonMesh.face[x].f[0] = (x) * 2;
			polygonMesh.face[x].f[1] = (x) * 2 + 1;
		}
		else
		{
			polygonMesh.vertexPos[x * 2 + 0].pos[0] = -Q;
			polygonMesh.vertexPos[x * 2 + 0].pos[1] = 0.0f;
			polygonMesh.vertexPos[x * 2 + 0].pos[2] = (x - (X_NUM /2)) * 1.0f - Q;

			polygonMesh.vertexPos[x * 2 + 1].pos[0] = Q;
			polygonMesh.vertexPos[x * 2 + 1].pos[1] = 0.0f;
			polygonMesh.vertexPos[x * 2 + 1].pos[2] = (x - (X_NUM /2)) * 1.0f - Q;

			polygonMesh.face[x].f[0] = (x) * 2;
			polygonMesh.face[x].f[1] = (x) * 2 + 1;
		}
//		LOG_INF("x: " << x << ", v1: " << floorVectors[x][0][0] << ", " << floorVectors[x][0][1] << ", " << floorVectors[x][0][2] << ", v2: " << floorVectors[x][1][0] << ", " << floorVectors[x][1][1] << ", " << floorVectors[x][1][2]);
//		LOG_INF("idx: " << indices[x][0] << ", " << indices[x][1]);
	}

	return OGLPolygonMesh::create<OGLPolygonMesh::LINE>(polygonMesh, 0);
}
