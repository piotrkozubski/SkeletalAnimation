/*
 * pmap.cpp
 *
 *  Created on: 17.07.2019
 *      Author: piotr.kozubski
 */

#include "Pmap.h"
#include "Logger.h"
#include "ParseUtils.h"
#include "Array.h"

#include <fstream>
#include <cstring>

Pmap* Pmap::createFromFile(const char* pFileName)
{
	Array<VertexPos>::Ptr   vertexPosList;
	Array<VertexUV>::Ptr    vertexUVList;
	Array< Face<3> >::Ptr 	faceList;
	Array<Normal>::Ptr  	faceNormalList;
    uint32_t   				verticesNum = 0;
    uint32_t   				faceNum = 0;
    uint32_t   				uvNum = 0;

    std::ifstream file;
    file.open(pFileName);
    std::string line;

    verticesNum = parseLineParam(file);
    faceNum = parseLineParam(file);
    uvNum = parseLineParam(file);

    if (static_cast<int>(verticesNum) <= 0 ||
            static_cast<int>(faceNum) <= 0)
    {
        LOG_ERR("Incorrect Vertices/Triangles number: " << verticesNum << "/" << faceNum);
        return NULL;
    }

    // Allocate vertices and triangles and texCoords
    LOG_INF("vertices number: " << verticesNum <<
            ", triangles number: " << faceNum <<
			", texture coordinates flag: " << uvNum);

    vertexPosList.reset(new Array<VertexPos>(verticesNum));
    vertexUVList.reset(new Array<VertexUV>(verticesNum));
    faceList.reset(new Array< Face<3> >(faceNum));
    faceNormalList.reset(new Array<Normal>(faceNum));

    // Read all vertices
    for (uint32_t i = 0; i < verticesNum; ++i)
    {
        if (!readLine(file, line))
        {
            LOG_ERR("Error reading vertices, vertex index: " << i);
            return NULL;
        }
        else
        {
        	//LOG_DET("line read: " << line);
        }

        char *p = std::strtok(const_cast<char*>(line.c_str()), ",");
        VertexPos vPos;
        vPos.pos[0] = parseFloat(p);
        vPos.pos[1] = parseFloat(p);
        vPos.pos[2] = parseFloat(p);

        memcpy(&vertexPosList->mArray[i], &vPos.pos[0], sizeof(VertexPos));

        if (uvNum)
        {
        	VertexUV vUV;
        	vUV.uv[0] = parseFloat(p);
        	vUV.uv[1] = parseFloat(p);

        	memcpy(&vertexUVList->mArray[i], &vUV.uv[0], sizeof(VertexUV));
        }

        LOG_DEB("position: " << i
        					 << ": " << vPos.pos[0]
                             << ", " << vPos.pos[1]
                             << ", " << vPos.pos[2]);

        if (uvNum)
        {
        	LOG_DEB("tex coord: " << i
        			<< ": " << vertexUVList->mArray[i].uv[0]
					<< ", " << vertexUVList->mArray[i].uv[1]);
        }
    }

    // TRIANGLES (VERTEX INDICES)
	for (uint32_t i = 0; i < faceNum; ++i)
	{
		if (!readLine(file, line))
		{
			LOG_ERR("Error reading indices, index: " << i);
			return NULL;
		}

		char *p = std::strtok(const_cast<char*>(line.c_str()), ",");
		Face<3> f;
		f.f[0] = parseInt(p);
		f.f[1] = parseInt(p);
		f.f[2] = parseInt(p);
		Normal fn;
		fn.n[0] = parseFloat(p);
		fn.n[1] = parseFloat(p);
		fn.n[2] = parseFloat(p);

		memcpy(&faceList->mArray[i], &f.f[0], sizeof(Face<3>));
		memcpy(&faceNormalList->mArray[i], &fn.n[0], sizeof(Normal));

		LOG_DEB("index: " << i << ": " << f.f[0]
						    	<< ", " << f.f[1]
								<< ", " << f.f[2]
								<< ", normal: "
								<< fn.n[0] << ", "
								<< fn.n[1] << ", "
								<< fn.n[2]);
	}

    file.close();
    return new Pmap(vertexPosList, vertexUVList, faceList, faceNormalList);
}

Pmap::Pmap(Array<VertexPos>::Ptr& 	 pVertexPosList,
		   Array<VertexUV>::Ptr& 	 pVertexUVList,
		   Array< Face<3> >::Ptr&       pFaceList,
		   Array<Normal>::Ptr&   pFaceNormalList) :
		mVertexPosList(std::move(pVertexPosList)),
		mVertexUVList(std::move(pVertexUVList)),
		mFaceList(std::move(pFaceList)),
		mFaceNormalList(std::move(pFaceNormalList))
{
	LOG_ASSERT(mVertexPosList.get() && mVertexPosList.get()->mArraySize, "Vertex Pos list is empty");
	LOG_ASSERT(mVertexUVList.get() && mVertexUVList.get()->mArraySize, "Vertex UV list is empty");
	LOG_ASSERT(mFaceList.get() && mFaceList.get()->mArraySize, "Face list is empty");
	LOG_ASSERT(mFaceNormalList.get() && mFaceNormalList.get()->mArraySize, "Face normal list is empty");
}


Pmap::~Pmap(void)
{

}

