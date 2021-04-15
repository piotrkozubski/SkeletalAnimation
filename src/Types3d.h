/*
 * Types3d.h
 *
 *  Created on: 23.07.2019
 *      Author: piotr.kozubski
 */

#ifndef TYPES3D_H
#define TYPES3D_H

#include <stdint.h>

typedef float       Vector2f[2];
typedef float       Vector3f[3];
typedef float		Vector4f[4];
typedef uint32_t    Polygon1i;
typedef uint32_t    Polygon2i[2];
typedef uint32_t    Polygon3i[3];
typedef uint32_t    Polygon4i[4];

typedef uint32_t 	BoneID;
typedef BoneID	 	VertexBoneID;
typedef float   	VertexBoneWeight;

/**
 *
 */
struct VertexPos
{
	Vector3f pos;
} __attribute__((packed));

/**
 *
 */
struct VertexUV
{
	Vector2f uv;
} __attribute__((packed));

/**
 *
 */
template <uint32_t size> struct Face
{
	Polygon1i f[size];
} __attribute__((packed));

/**
 *
 */
struct Normal
{
	Vector3f n;
} __attribute__((packed));

#endif /* TYPES3D_H */
