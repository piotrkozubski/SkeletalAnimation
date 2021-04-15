/*
 * Math3d.h
 *
 *  Created on: Apr 16, 2013
 *      Author: pkozubsk
 */

#ifndef MATH3D_H_
#define MATH3D_H_

#include "Types3d.h"

/**
 * Math 3d utilities name-space
 *
 * Math3d name-space should contain non-member functions which implements
 * separate algorithms specific for 3d math (e.g. vectors, matrices operations)
 *
 */
namespace Math3d
{
    /**
     *  Dot product of two vectors. Result is a cos(alpha).
     *
     *  @param  pV1     first vector
     *  @param  pV2     second vector
     *
     *  @return         float number representing cos(alpha)
     */
    inline float dotProduct3f(const Vector3f& pV1, const Vector3f& pV2)
    {
        return (pV1[0] * pV2[0] + pV1[1] * pV2[1] + pV1[2] * pV2[2]);
    }

    /**
     * Cross product of two vectors. Result is normal vector.
     *
     *  @param  pRes    result vector
     *  @param  pV1     first vector
     *  @param  pV2     second vector
     */
    inline void crossProduct3f(Vector3f pRes,
                               const Vector3f pV1,
                               const Vector3f pV2)
    {
        pRes[0] = pV1[1] * pV2[2] - pV1[2] * pV2[1];
        pRes[1] = pV1[0] * pV2[2] - pV1[2] * pV2[0];
        pRes[1] = pV1[0] * pV2[1] - pV1[1] * pV2[0];
    }

    /**
     * Tells if vertex 1 is behind the vertex 2
     *
     * @param pTriangle1	first vertex
     * @param pTriangle2	second vertex
     * @param pNormal		normal vector of second vertex
     *
     * @return bool			'true' if triangle is behind
     */
    bool isVertexBehind(const VertexPos& pVertex1,
	 	     	 	 	const VertexPos& pVertex2,
						const Normal& pNormal);

    /**
     * Normalizes length the 3d vector
     * vector lenght after normalization [0, 1]
     *
     * @param v		reference to 3d vector
     *
     * @return 		reference to 3d vector
     */
    Vector3f& normalizeVector3f(Vector3f& v);

    /**
     * Checks if two triangles intersects
     * Uses Line-Triangle algorithm in two ways
     *
     * @param pTrian1	pointer to 3 vertices of first triangle
     * @param pTrian2	pointer to 3 vertices of second triangle
     *
     * @return bool		'true' if inetrsects
     */
    bool doTrianglesIntersect(const VertexPos* pTrian1, const VertexPos* pTrian2);
}

#endif /* MATH3D_H_ */
