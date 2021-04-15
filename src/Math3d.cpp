/*
 * Math3d.cpp
 *
 *  Created on: Apr 16, 2013
 *      Author: pkozubsk
 */
#include "Math3d.h"
#include "Logger.h"
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>

Vector3f& Math3d::normalizeVector3f(Vector3f& v)
{
	float l = sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
	v[0] = v[0] / l;
	v[1] = v[1] / l;
	v[2] = v[2] / l;
	return v;
}

bool Math3d::isVertexBehind(const VertexPos& pVertex1,
						 	const VertexPos& pVertex2,
							const Normal& pNormal)
{
	Vector3f norm = { pNormal.n[0], pNormal.n[1], pNormal.n[2] };
	Vector3f norm2;
	norm2[0] = pVertex2.pos[0] - pVertex1.pos[0];
	norm2[1] = pVertex2.pos[1] - pVertex1.pos[1];
	norm2[2] = pVertex2.pos[2] - pVertex1.pos[2];
	normalizeVector3f(norm2);
	normalizeVector3f(norm);

	return (dotProduct3f(norm, norm2) < -0.000001);
}

bool Math3d::doTrianglesIntersect(const VertexPos* pTrian1, const VertexPos* pTrian2)
{
	glm::vec3 intersec;
	// TRIAN1
	glm::vec3 p0(pTrian1[0].pos[0], pTrian1[0].pos[1], pTrian1[0].pos[2]);
	glm::vec3 p1(pTrian1[1].pos[0], pTrian1[1].pos[1], pTrian1[1].pos[2]);
	glm::vec3 p2(pTrian1[2].pos[0], pTrian1[2].pos[1], pTrian1[2].pos[2]);

	// TRIAN1 - LINE TRIAN2 V1-V2
	glm::vec3 s1(pTrian2[0].pos[0], pTrian2[0].pos[1], pTrian2[0].pos[2]);
	glm::vec3 s2(pTrian2[1].pos[0], pTrian2[1].pos[1], pTrian2[1].pos[2]);

	bool result = glm::intersectLineTriangle(s1, s2, p0, p1, p2, intersec);

	// TRIAN1 - LINE TRIAN2 V1-V3
	s1[0] = pTrian2[0].pos[0]; s1[1] = pTrian2[0].pos[1]; s1[2] = pTrian2[0].pos[2];
	s2[0] = pTrian2[2].pos[0]; s2[1] = pTrian2[2].pos[1]; s2[2] = pTrian2[2].pos[2];
	result |= glm::intersectLineTriangle(s1, s2, p0, p1, p2, intersec);

	// TRIAN1 - LINE TRIAN2 V2-V3
	s1[0] = pTrian2[1].pos[0]; s1[1] = pTrian2[1].pos[1]; s1[2] = pTrian2[1].pos[2];
	s2[0] = pTrian2[2].pos[0]; s2[1] = pTrian2[2].pos[1]; s2[2] = pTrian2[2].pos[2];
	result |= glm::intersectLineTriangle(s1, s2, p0, p1, p2, intersec);

	// TRIAN2
	p0[0] = pTrian2[0].pos[0]; p0[1] = pTrian2[0].pos[1]; p0[2] = pTrian2[0].pos[2];
	p1[0] = pTrian2[1].pos[0]; p0[1] = pTrian2[1].pos[1]; p0[2] = pTrian2[1].pos[2];
	p2[0] = pTrian2[2].pos[0]; p0[1] = pTrian2[2].pos[1]; p0[2] = pTrian2[2].pos[2];

	// TRIAN2 - LINE TRIAN1 V1-V2
	s1[0] = pTrian1[0].pos[0]; s1[1] = pTrian1[0].pos[1]; s1[2] = pTrian1[0].pos[2];
	s2[0] = pTrian1[1].pos[0]; s2[1] = pTrian1[1].pos[1]; s2[2] = pTrian1[1].pos[2];
	result |= glm::intersectLineTriangle(s1, s2, p0, p1, p2, intersec);

	// TRIAN2 - LINE TRIAN1 V1-V3
	s1[0] = pTrian1[0].pos[0]; s1[1] = pTrian1[0].pos[1]; s1[2] = pTrian1[0].pos[2];
	s2[0] = pTrian1[2].pos[0]; s2[1] = pTrian1[2].pos[1]; s2[2] = pTrian1[2].pos[2];
	result |= glm::intersectLineTriangle(s1, s2, p0, p1, p2, intersec);

	// TRIAN2 - LINE TRIAN1 V2-V3
	s1[0] = pTrian1[1].pos[0]; s1[1] = pTrian1[1].pos[1]; s1[2] = pTrian1[1].pos[2];
	s2[0] = pTrian1[2].pos[0]; s2[1] = pTrian1[2].pos[1]; s2[2] = pTrian1[2].pos[2];
	result |= glm::intersectLineTriangle(s1, s2, p0, p1, p2, intersec);

	return result;
}
