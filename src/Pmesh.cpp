/*
 * Pmesh.cpp
 *
 *  Created on: 14.06.2019
 *      Author: piotr.kozubski
 */

#include "Pmesh.h"
#include "OGLPolygonMesh.h"
#include "Logger.h"
#include "Bone.h"
#include "Matrix.h"

#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>

#define BONES_PER_VERTEX	4

static bool  readPMeshLine(std::ifstream& is, std::string& s);
static int   parseInt(char *& p);
static float parseFloat(char *& p);
static int 	 getLineParam(std::ifstream& file);
static bool  parseBones(std::ifstream& pFile, Array<Bone::Ptr>::Ptr& pBoneList);
static bool  parseTriangles(std::ifstream& pFile, std::vector< Face<3> >& pTriangles, uint32_t pTrianglesNum);
static bool  parseKeyFrames(std::ifstream& pFile, Array<RawKeyFrame>::Ptr& pKeyFrames);
static Matrix4x4f parseMatrix4x4f(char *& p);

typedef VertexBoneID PMESH_VertexBonesIDs[BONES_PER_VERTEX];
typedef VertexBoneWeight PMESH_VertexBonesWeights[BONES_PER_VERTEX];

Pmesh* Pmesh::createFromFile(const char* pmeshFileName)
{
	PolygonMesh<3> polygonMesh;
    uint32_t verticesNum = 0;
    uint32_t trianglesNum = 0;
    uint32_t normalsFlag = 0;
    uint32_t texCoordFlag = 0;
    uint32_t bonesRefsFlag = 0;

    std::ifstream file;
    file.open(pmeshFileName);
    std::string line;


    verticesNum = getLineParam(file);
    trianglesNum = getLineParam(file);
    normalsFlag = getLineParam(file);
    texCoordFlag = getLineParam(file);
    bonesRefsFlag = getLineParam(file);

    if (static_cast<int>(verticesNum) <= 0 ||
            static_cast<int>(trianglesNum) <= 0)
    {
        LOG_ERR("Incorrect Vertices/Triangles number: " << verticesNum << "/" << trianglesNum);
        return NULL;
    }

    // Allocate vertices and triangles and texCoords
    LOG_INF("vertices number: " << verticesNum <<
            ", triangles number: " << trianglesNum <<
			", normals flag: " << normalsFlag <<
			", texture coordinates flag: " << texCoordFlag <<
			", bonesFlag:" << bonesRefsFlag);
    polygonMesh.vertexPos.resize(verticesNum);
    polygonMesh.face.resize(trianglesNum);
    if (normalsFlag)
    {
    	polygonMesh.vertexNormal.resize(verticesNum);
    }
    if (texCoordFlag)
    {
    	polygonMesh.vertexUV.resize(verticesNum);
    }
    if (bonesRefsFlag)
    {
    	polygonMesh.vertexBoneID.resize(verticesNum * BONES_PER_VERTEX);
    	polygonMesh.vertexBoneWeight.resize(verticesNum * BONES_PER_VERTEX);
    }

    // Read all vertices
    for (uint32_t i = 0; i < verticesNum; ++i)
    {
        if (!readPMeshLine(file, line))
        {
            LOG_ERR("Error reading vertices, vertex index: " << i);
            return NULL;
        }
        else
        {
        	//LOG_DET("line read: " << line);
        }

        char *p = std::strtok(const_cast<char*>(line.c_str()), ",");
        polygonMesh.vertexPos[i].pos[0] = parseFloat(p);
        polygonMesh.vertexPos[i].pos[1] = parseFloat(p);
        polygonMesh.vertexPos[i].pos[2] = parseFloat(p);

        if (normalsFlag) //if normals defined
        {
        	polygonMesh.vertexNormal[i].n[0] = parseFloat(p);
        	polygonMesh.vertexNormal[i].n[1] = parseFloat(p);
        	polygonMesh.vertexNormal[i].n[2] = parseFloat(p);
        }
        if (texCoordFlag) //if texture coordinates defined
        {
        	polygonMesh.vertexUV[i].uv[0] = parseFloat(p);
        	polygonMesh.vertexUV[i].uv[1] = parseFloat(p);
        }
        if (bonesRefsFlag) // if bones + animation is defined: every vertex has offset to 2 bones + weights for them
        {
        	// FIRST and SECOND BONE ID
        	for (int bone = 0; bone < BONES_PER_VERTEX; ++bone)
        	{

        		int idx = parseInt(p);
        		if (idx == -1) idx = 0xFF;
        		polygonMesh.vertexBoneID[i * BONES_PER_VERTEX + bone] = idx & 0x000000FF;
        	}
        	// FIRST and SECOND BONE WEIGHT
        	for (int bone = 0; bone < BONES_PER_VERTEX; ++bone)
        	{
        		polygonMesh.vertexBoneWeight[i * BONES_PER_VERTEX + bone] = parseFloat(p);
        	}
        }

        LOG_DEB("position: " << i << ": " << polygonMesh.vertexPos[i].pos[0]
                           << ", " << polygonMesh.vertexPos[i].pos[1]
                           << ", " << polygonMesh.vertexPos[i].pos[2]);
        if (normalsFlag)
        {
        	LOG_DEB("normal: " << i << ": " << polygonMesh.vertexNormal[i].n[0]
						       << ", " << polygonMesh.vertexNormal[i].n[1]
						       << ", " << polygonMesh.vertexNormal[i].n[2]);
        }
        if (texCoordFlag)
        {
        	LOG_DEB("tex coord: " << i << ": " << polygonMesh.vertexUV[i].uv[0]
                                   << ", " << polygonMesh.vertexUV[i].uv[1]);
        }
        if (bonesRefsFlag)
        {
        	LOG_DEB("bone ids for vertex " << i << ": " << (unsigned)polygonMesh.vertexBoneID[i * BONES_PER_VERTEX + 0]
                     << ", " << (unsigned)polygonMesh.vertexBoneID[i * BONES_PER_VERTEX + 1]);
        	LOG_DEB("bone weights for vertex: " << i << ": " << polygonMesh.vertexBoneWeight[i * BONES_PER_VERTEX + 0]
                     << ", " << polygonMesh.vertexBoneWeight[i * BONES_PER_VERTEX + 1]);
        }
    }

    // TRIANGLES (VERTEX INDICES)
    if (!parseTriangles(file, polygonMesh.face, trianglesNum))
    {
    	return NULL;
    }

    OGLPolygonMesh *mesh = OGLPolygonMesh::
    		create<OGLPolygonMesh::TRIANGLE>(polygonMesh, BONES_PER_VERTEX);

//    OGLPolygonMesh *mesh = OGLPolygonMesh::create(positions.get(),
//    							  	  	   normals.get(),
//										   texCoords.get(),
//										   BONES_PER_VERTEX,
//										   &bonesIDs.get()[0][0],
//										   &bonesWeights.get()[0][0],
//										   verticesNum,
//										   triangles.get(), trianglesNum);
    LOG_ASSERT(mesh,  "Cannot create OGLPolygonMesh object!");

    BoneKeyFrames *boneKFrames = NULL;

    // BONES
    if (bonesRefsFlag)
    {
    	Array<Bone::Ptr>::Ptr boneList;
    	if (!parseBones(file, boneList))
    	{
    		return NULL;
    	}

    	// KEY FRAMES
    	uint32_t framesNum = getLineParam(file);
    	LOG_INF("framesNum: " << framesNum);

    	Array<RawKeyFrame>::Ptr keyFrames;
    	if (!parseKeyFrames(file, keyFrames))
    	{
    		return NULL;
    	}

    	boneKFrames = BoneKeyFrames::create(framesNum, boneList, keyFrames);
    	LOG_ASSERT(mesh,  "Cannot create BoneKeyFrames object!");
    }

    file.close();
    return new Pmesh(mesh, boneKFrames);
}

static bool readPMeshLine(std::ifstream& is, std::string& s)
{
	bool result = std::getline(is, s);
	if (result)
	{
		if (s.size() && s.at(0) == '#') result = std::getline(is, s);
	}
	return result;
}

static int parseInt(char *& p)
{
	int value;
	if (p)
	{
		value = std::atoi(p);
		p = std::strtok(NULL, ",");
	}

	return value;
}

static float parseFloat(char *& p)
{
	float value;
	if (p)
	{
		value = std::atof(p);
		p = std::strtok(NULL, ",");
	}

	return value;
}

static Matrix4x4f parseMatrix4x4f(char *& p)
{
	Matrix4x4f matrix;
	for (uint32_t row = 0; row < 4; ++row)
	{
		for (uint32_t col = 0; col < 4; ++col)
		{
			matrix[col][row] = parseFloat(p);
		}
	}
	return matrix;
}

static int getLineParam(std::ifstream& file)
{
	std::string line;
    if (readPMeshLine(file, line))
    {
        return std::atoi(line.c_str());
    }
    else
    {
    	LOG_ERR("Error parsing parameter from PMESH file.");
    }
    return 0;
}

static bool parseBones(std::ifstream& pFile, Array<Bone::Ptr>::Ptr& pBoneList)
{
    uint32_t bonesNum = getLineParam(pFile);
    LOG_INF("bones number: " << bonesNum);
    pBoneList.reset(new Array<Bone::Ptr>(bonesNum));

	std::string line;
	// Read all bones
	for (uint32_t i = 0; i < bonesNum; ++i)
	{
		if (!readPMeshLine(pFile, line))
		{
			LOG_ERR("Error reading bones, bone index: " << i);
			return false;
		}
		char *p = std::strtok(const_cast<char*>(line.c_str()), ",");

		int readVal = parseInt(p);	// parent index
		uint16_t parent = (readVal == -1) ? Bone::NO_PARENT_IDX : readVal;
		Matrix4x4f m = parseMatrix4x4f(p);
		pBoneList->mArray[i].reset(new Bone(m, parent));
	}

	return true;
}

static bool parseTriangles(std::ifstream& pFile, std::vector< Face<3> >& pTriangles, uint32_t pTrianglesNum)
{
	std::string line;
	for (uint32_t i = 0; i < pTrianglesNum; ++i)
	{
		if (!readPMeshLine(pFile, line))
		{
			LOG_ERR("Error reading indices, index: " << i);
			return false;
		}

		char *p = std::strtok(const_cast<char*>(line.c_str()), ",");
		pTriangles[i].f[0] = parseInt(p);
		pTriangles[i].f[1] = parseInt(p);
		pTriangles[i].f[2] = parseInt(p);

		LOG_DEB("index: " << i << ": " << pTriangles[i].f[0]
						    	<< ", " << pTriangles[i].f[1]
								<< ", " << pTriangles[i].f[2]);
	}

	return true;
}

static bool parseKeyFrames(std::ifstream& pFile, Array<RawKeyFrame>::Ptr& pKeyFrames)
{
	uint32_t keyFramesNum = getLineParam(pFile);
	LOG_INF("Key Frames number: " << keyFramesNum);
	LOG_ASSERT(keyFramesNum, "key frames num not set");

	pKeyFrames.reset(new Array<RawKeyFrame>(keyFramesNum));

	std::string line;
	for (uint32_t i = 0; i < keyFramesNum; ++i)
	{
		if (!readPMeshLine(pFile, line))
		{
			LOG_ERR("Error key frames, index: " << i);
			return false;
		}
		char *p = std::strtok(const_cast<char*>(line.c_str()), ",");
		RawKeyFrame rkf = {0};
		rkf.boneId = parseInt(p) & 0xFF;
		rkf.frameId = parseInt(p) & 0xFFFFFFFF;
		if (rkf.boneId == 0)
		{
			rkf.posOffset[0] = parseFloat(p);
			rkf.posOffset[1] = parseFloat(p);
			rkf.posOffset[2] = parseFloat(p);
		}
		rkf.rotQuat[0] = parseFloat(p);
		rkf.rotQuat[1] = parseFloat(p);
		rkf.rotQuat[2] = parseFloat(p);
		rkf.rotQuat[3] = parseFloat(p);

		LOG_DEB("boneID: " << (unsigned)rkf.boneId << ", frameID: " << rkf.frameId <<
				", pos offset: " << rkf.posOffset[0] << ", " << rkf.posOffset[1] << ", " << rkf.posOffset[2] <<
				", rot Quat: " << rkf.rotQuat[0] << ", " << rkf.rotQuat[1] << ", " << rkf.rotQuat[2] << ", " << rkf.rotQuat[3]);

		pKeyFrames->mArray[i] = rkf;
	}
	return true;
}
