/*
 * GridMesh.h
 *
 *  Created on: 13.05.2019
 *      Author: piotr.kozubski
 */

#ifndef GRIDMESH_H_
#define GRIDMESH_H_

class OGLPolygonMesh;

/**
 * GridMesh class.
 *
 * Gird Mesh is responsible for creating 3d grid in the space.
 * That's can be useful as a reference surface
 */
class GridMesh
{
public:
	/**
	 * Creates OGLPolygonMesh for Grid
	 *
	 * @return pointer to OGLPolygonMesh object
	 */
	static OGLPolygonMesh* create(void);
};

#endif /* GRIDMESH_H_ */
