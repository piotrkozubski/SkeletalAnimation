/*
 * OGLPolygonMesh.cpp
 *
 *  Created on: Apr 23, 2013
 *      Author: pkozubsk
 */

#include "OGLFunctions.h"
#include "OGLPolygonMesh.h"
#include "OGLUtils.h"
#include "OGLProgramAttrib.h"
#include "Logger.h"

template <OGLPolygonMesh::Type type>
OGLPolygonMesh* OGLPolygonMesh::create(const PolygonMesh<type>& pPolygonMesh, uint8_t pBonesPerVertex)
{
    using namespace OGLUtils;

    uint32_t verticesNum = pPolygonMesh.vertexPos.size();
    LOG_DET("OGLPolygonMesh::create VerticesNum: " << verticesNum << ", IdxObjNum: " << pPolygonMesh.face.size());

    uint32_t sizeOfPos = (sizeof(Vector3f)) * verticesNum;
    uint32_t sizeOfIndices = sizeof(pPolygonMesh.face[0]) * pPolygonMesh.face.size();
    uint32_t sizeOfNormals = (pPolygonMesh.vertexNormal.size() ? (sizeof(Vector3f)) * verticesNum : 0);
    uint32_t sizeOfTexCoord = (pPolygonMesh.vertexUV.size() ? (sizeof(Vector2f)) * verticesNum : 0);
    uint32_t sizeOfBonesIDs = (pPolygonMesh.vertexBoneID.size() ? (sizeof(VertexBoneID) * pBonesPerVertex) * verticesNum : 0);
    uint32_t sizeOfBonesWeights = (pPolygonMesh.vertexBoneWeight.size() ? (sizeof(VertexBoneWeight) * pBonesPerVertex) * verticesNum : 0);

    GLintptr offsetOfNormals = sizeOfPos;
    GLintptr offsetOfTexCoord = offsetOfNormals + sizeOfNormals;
    GLintptr offsetOfBonesIDs = offsetOfTexCoord + sizeOfTexCoord;
    GLintptr offsetOfBonesWeights = offsetOfBonesIDs + sizeOfBonesIDs;

    LOG_DET("Vertex attributes \"Size of\" and offsets: ");
    LOG_DET("sizeOfPos: " << sizeOfPos);
    LOG_DET("sizeOfIndices: " << sizeOfIndices);
    LOG_DET("sizeOfNormals: " << sizeOfNormals);
    LOG_DET("sizeOfTexCoord: " << sizeOfTexCoord);
    LOG_DET("sizeOfBonesIDs: " << sizeOfBonesIDs);
    LOG_DET("sizeOfBonesWeights: " << sizeOfBonesWeights);
    LOG_DET("offsetOfNormals: " << (uint32_t)offsetOfNormals);
    LOG_DET("offsetOfTexCoord: " << (uint32_t)offsetOfTexCoord);
    LOG_DET("offsetOfBonesIDs: " << (uint32_t)offsetOfBonesIDs);
    LOG_DET("offsetOfBonesWeights: " << (uint32_t)offsetOfBonesWeights);

    LOG_DET("Vertex attributes addresses:");
    LOG_DET("pPositions: " << &pPolygonMesh.vertexPos[0]);
    LOG_DET("pNormals: " << &pPolygonMesh.vertexNormal[0]);
    LOG_DET("pTexCoords: " << &pPolygonMesh.vertexUV[0]);
    LOG_DET("pBonesIDs: " << &pPolygonMesh.vertexBoneID[0]);
    LOG_DET("pBonesWeights: " << &pPolygonMesh.vertexBoneWeight[0]);
    LOG_DET("pIdxObj: " << &pPolygonMesh.face[0]);

    LOG_DET("sizeOfIndices: " << uint32_t(sizeOfIndices) << ", " << sizeof(pPolygonMesh.face[0]) << ", " << pPolygonMesh.face.size());

    // define VAO
    GLuint vertArray;
    glGenVertexArrays(1, &vertArray);
    glBindVertexArray(vertArray);

    // define Array Buffer
    GLuint vertBuffer;
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);

    glBufferData(GL_ARRAY_BUFFER,
    		sizeOfPos +
			sizeOfNormals +
			sizeOfTexCoord +
			sizeOfBonesIDs +
			sizeOfBonesWeights,
			NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfPos, &pPolygonMesh.vertexPos[0]);

    if (pPolygonMesh.vertexNormal.size())
    	glBufferSubData(GL_ARRAY_BUFFER, offsetOfNormals, sizeOfNormals, &pPolygonMesh.vertexNormal[0]);
    if (pPolygonMesh.vertexUV.size())
    	glBufferSubData(GL_ARRAY_BUFFER, offsetOfTexCoord, sizeOfTexCoord, &pPolygonMesh.vertexUV[0]);
    if (pPolygonMesh.vertexBoneID.size())
    	glBufferSubData(GL_ARRAY_BUFFER, offsetOfBonesIDs, sizeOfBonesIDs, &pPolygonMesh.vertexBoneID[0]);
    if (pPolygonMesh.vertexBoneWeight.size())
    	glBufferSubData(GL_ARRAY_BUFFER, offsetOfBonesWeights, sizeOfBonesWeights, &pPolygonMesh.vertexBoneWeight[0]);

    glVertexAttribPointer(OGLProgramAttrib::ATTRIBIDX_VERTPOS, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(OGLProgramAttrib::ATTRIBIDX_VERTPOS);

    if (pPolygonMesh.vertexNormal.size())
    {
    	glVertexAttribPointer(OGLProgramAttrib::ATTRIBIDX_VERTNORM,
    			3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(const GLvoid* )(sizeOfPos));
    	LOG_DEB("Normals loading error status: " << glGetError());
    	glEnableVertexAttribArray(OGLProgramAttrib::ATTRIBIDX_VERTNORM);
    	LOG_DEB("Normals loading error status: " << glGetError());
    }

    if (pPolygonMesh.vertexUV.size())
    {
    	glVertexAttribPointer(OGLProgramAttrib::ATTRIBIDX_VERTTEXCOORD,
    			2,
				GL_FLOAT,
				GL_FALSE,
				0,
				(const GLvoid* )(offsetOfTexCoord));
    	LOG_DEB("Tex coords loading error status: " << glGetError());
    	glEnableVertexAttribArray(OGLProgramAttrib::ATTRIBIDX_VERTTEXCOORD);
    	LOG_DEB("Tex coords loading error status: " << glGetError());
    }

    if (pPolygonMesh.vertexBoneID.size())
    {
    	glVertexAttribIPointer(OGLProgramAttrib::ATTRIBIDX_BONEIDXS,
    			4,
				GL_UNSIGNED_INT,
				0,
				(const GLvoid* )(offsetOfBonesIDs));
    	LOG_DEB("Bones IDs loading error status: " << glGetError());
    	glEnableVertexAttribArray(OGLProgramAttrib::ATTRIBIDX_BONEIDXS);
    	LOG_DEB("Bones IDs loading error status: " << glGetError());
    }

    if (pPolygonMesh.vertexBoneWeight.size())
    {
    	glVertexAttribPointer(OGLProgramAttrib::ATTRIBIDX_BONEWEIGHTS,
    			4,
				GL_FLOAT,
				GL_FALSE,
				0,
				(const GLvoid* )(offsetOfBonesWeights));
    	LOG_DEB("Bones Weights loading error status: " << glGetError());
    	glEnableVertexAttribArray(OGLProgramAttrib::ATTRIBIDX_BONEWEIGHTS);
    	LOG_DEB("Bones Weights loading error status: " << glGetError());
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // define Indices Buffer
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, &pPolygonMesh.face[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    OGLPolygonMesh* polyMeshObj =
            new OGLPolygonMesh(vertArray, vertBuffer, verticesNum,
            		indexBuffer, pPolygonMesh.face.size(), type);

    return polyMeshObj;
}

OGLPolygonMesh::OGLPolygonMesh(GLuint vertexArray,
							   GLuint vertexBuffer, uint32_t vertexNum,
							   GLuint triangleBuffer, uint32_t triangleNum,
							   Type type) :
    mVertexArray(vertexArray),
    mVertexBuffer(vertexBuffer),
    mVertexNum(vertexNum),
    mTriangleBuffer(triangleBuffer),
	mTriangleNum(triangleNum),
	mType(type)
{
    LOG_INF("OpenGL Polygon Mesh created "
                 "(vertex array id:" << mVertexArray <<
                 ", vertex buffer id:" << mVertexBuffer <<
                 ", triangle buffer id:" << mTriangleBuffer << ")");
}

OGLPolygonMesh::~OGLPolygonMesh()
{
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mTriangleBuffer);
    glDeleteVertexArrays(1, &mVertexArray);

    LOG_INF("OpenGL Polygon Mesh deleted "
                 "(vertex array id:" << mVertexArray <<
                 ", vertex buffer id:" << mVertexBuffer <<
    			 ", triangle buffer id:" << mTriangleBuffer << ")");
}

void OGLPolygonMesh::draw(void) const
{
    glBindVertexArray(mVertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTriangleBuffer);
    if (mType == TRIANGLE)
    	glDrawElements(GL_TRIANGLES, mTriangleNum * 3, GL_UNSIGNED_INT, 0);
    else if (mType == LINE)
        	glDrawElements(GL_LINES, mTriangleNum * 2, GL_UNSIGNED_INT, 0);
    else if (mType == POINT)
            glDrawElements(GL_POINTS, mTriangleNum, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// generate template functions implementations
static void temporaryFunc(void)
{
	PolygonMesh<OGLPolygonMesh::TRIANGLE> pmTriangle;
	PolygonMesh<OGLPolygonMesh::LINE> pmLine;
	PolygonMesh<OGLPolygonMesh::POINT> pmPoint;
	OGLPolygonMesh::create<OGLPolygonMesh::TRIANGLE>(pmTriangle, 0);
	OGLPolygonMesh::create<OGLPolygonMesh::LINE>(pmLine, 0);
	OGLPolygonMesh::create<OGLPolygonMesh::POINT>(pmPoint, 0);
}
