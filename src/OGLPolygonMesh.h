/*
 * OGLPolygonMesh.h
 *
 *  Created on: Apr 18, 2013
 *      Author: pkozubsk
 */

#ifndef OGLPOLYGONMESH_H_
#define OGLPOLYGONMESH_H_

#include <memory>
#include <stdint.h>
#include <GL/glcorearb.h>
#include "Bone.h"
#include "PolygonMesh.h"

/**
 * OGLPolygonMesh class.
 *
 * OpenGL Polygon Mesh is responsible for rendering polygon mesh.
 */
class OGLPolygonMesh
{
public:

	/**
	 * Type of primitive used by mesh
	 */
	enum Type
	{
		POINT = 1,
		LINE = 2,
		TRIANGLE = 3
	};

    /**
     *  Static member function responsible for OGLPolygonMesh object creation.
     *
     *  @param  pPositions			pointer to vector structure
     *  @param  pNormals			pointer to vector structure
     *  @param  pTexCoords			pointer to vector structure
     *  @param 	pBonesPerVertex		number of bones references per vertex
     *  @param  pBonesRefs			pointer to bone reference structure
     *  @param  pVerticesNum		number of vertices
     *  @param  pIndices			pointer to indices array
     *  @param  pIndicesNum			number of objects (triangles or quads)
     *  @param  pPosAttribIdx    	vertex array index of position attribute
     *  @param  pTexCoordAttribIdx	vertex array index of position attribute
     *  @param  type				type of polygon mesh. see Type enumerator
     *
     *  @return                 	pointer to OGLPolyonMesh object
     */
	template <OGLPolygonMesh::Type type>
	static OGLPolygonMesh* create(const PolygonMesh<type>& pPolygonMesh, uint8_t pBonesPerVertex);

    /**
     *  Destructor
     */
    ~OGLPolygonMesh();

    /**
     *  Draws polygon mesh on the frame buffer.
     */
    void draw(void) const;

    /**
     * typedef of smart pointer to OGLPolygonMesh
     */
    typedef std::auto_ptr<OGLPolygonMesh> Ptr;

private:
    GLuint      mVertexArray;
    GLuint      mVertexBuffer;
    uint32_t    mVertexNum;
    GLuint      mTriangleBuffer;
    uint32_t    mTriangleNum;
    Type		mType;

    /**
     * Private constructor.
     *
     * @param   vertexArray       vertex array id
     * @param   vertexBuffer      vertex buffer id
     * @param   vertexNum         number of vertices
     * @param   triangleBuffer    triangle buffer id
     * @param   triangleNum       number of triangles
     * @param  	type			  type of polygon mesh. see Type enumerator
     */
    OGLPolygonMesh(GLuint vertexArray, GLuint vertexBuffer, uint32_t vertexNum,
    			   GLuint triangleBuffer , uint32_t triangleNum, Type type = TRIANGLE);
};

#endif /* OGLPOLYGONMESH_H_ */
