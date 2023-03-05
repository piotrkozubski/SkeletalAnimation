/*
 * BoneKeyFrames.cpp
 *
 *  Created on: 22.06.2019
 *      Author: zokp
 */

#include "BoneKeyFrames.h"
#include "Matrix.h"
#include <list>
#include <map>
#include <cstring>
#include <cstdlib>
#include <cmath>
//#define GLM_ENABLE_EXPERIMENTAL
//#include "..\inc\glm\gtx\string_cast.hpp"

#define SIZE_OF_MATRIX		64
#define MAX_NUM_OF_BONES	63

static void fillBoneChildren(Array<Bone::Ptr>& pBones);
static void fillKeyFrameMap(Array<Bone::Ptr>& pBoneList,
							Array<KeyFrameMapElement>& pMap,
							Array<RawKeyFrame>& pKeyFrames,
							FrameID pFrameNum);
static void sortRawKeyFrames(Array<RawKeyFrame*>& array);
static int RawKeyFrameCMP(const void * a, const void * b);

typedef std::map<FrameID, std::list<RawKeyFrame*> > MapType;
static RawKeyFrame* findPrevKeyFrame(BoneID bId, std::list<RawKeyFrame*>& list);

BoneKeyFrames* BoneKeyFrames::create(
		uint32_t pFramesNum, Array<Bone::Ptr>::Ptr pBoneList,
		Array<RawKeyFrame>::Ptr pKeyFrames)
{
	LOG_ASSERT(pFramesNum, "pFramesNum not set");
	LOG_ASSERT(pBoneList.get(), "pBoneList not set");
	LOG_ASSERT(pKeyFrames.get(), "pKeyFrames not set");
	LOG_INF("Creating BoneKeyFrames object...");

	// filling mChildren in bones
	fillBoneChildren(*pBoneList.get());

	LOG_INF("Allocating memory for key frames map...");
	Array<KeyFrameMapElement>::Ptr map(new Array<KeyFrameMapElement>(pFramesNum));
	fillKeyFrameMap(*pBoneList.get(), *map.get(), *pKeyFrames.get(), pFramesNum);

	return new BoneKeyFrames(pFramesNum, std::move(pBoneList), std::move(pKeyFrames), std::move(map));
}

BoneKeyFrames::BoneKeyFrames(
					uint32_t 						    pFramesNum,
					Array<Bone::Ptr>::Ptr&& 			pBoneList,
					Array<RawKeyFrame>::Ptr&&			pKeyFrames,
					Array<KeyFrameMapElement>::Ptr&&	pMap) :
		mFramesNum(pFramesNum),
		mBoneList(std::move(pBoneList)),
		mKeyFrames(std::move(pKeyFrames)),
		mMap(std::move(pMap))
{
	LOG_ASSERT(mFramesNum, 		 "mFramesNum not set");
	LOG_ASSERT(mBoneList.get(),  "mBoneList not set");
	LOG_ASSERT(mKeyFrames.get(), "mKeyFrames not set");
	LOG_ASSERT(mMap.get(), 		 "mMap not set");
	LOG_INF("BoneKeyFrames created (" << mFramesNum << " frames, " << mBoneList->mArraySize << " bones, "
			<< mKeyFrames->mArraySize << " key frames, " << mMap->mArraySize << " map frames)");

#if 0	//FOR DEBUG
	LOG_INF("Bone Key Frames Mapping:")
	for (FrameID frameId = 0; frameId < mMap->mArraySize; ++frameId)
	{
		KeyFrameMapElement& el = mMap->mArray[frameId];
		LOG_INF("Frame ID: " << frameId << ", adress: " << (void*)&el << ", next Key Frame: " << el.mNext << ", previous Key Frame: " << el.mPrevoius <<
				", raw Key Frames number: " << ((el.mKeyFrame.get() != 0) ? el.mKeyFrame->mArraySize : 0));
	}
#endif
}

BoneKeyFrames::~BoneKeyFrames(void)
{
}

void BoneKeyFrames::getFrame(uint8_t *pData, uint32_t pStart, uint32_t pEnd, float pFactor)
{

}

void BoneKeyFrames::getFrame(uint8_t *pData, float pFactor)
{
	if (pFactor > 1.0) pFactor = 1.0;
	float frameFloat = pFactor * (mFramesNum - 1);
	uint32_t frame = ceil(frameFloat);
	float factor = 1.0 - (float(frame) - frameFloat);

	Bone::Ptr* bl = mBoneList->mArray;
	for (BoneID id = 0; id < mBoneList->mArraySize; ++id)
	{
		Matrix::setIdentityMatrix4x4f(bl[id]->mModificationMatrix);
	}

	LOG_DEB("pFactor: " << pFactor << ", frame: " << frame << ", mMap array size: " << mMap->mArraySize);
	KeyFrameMapElement* element = &mMap->mArray[frame];

	if (element->mKeyFrame.get() == NULL)
	{
		element = element->mNext;
	}

	// endFrame commented out, but left in case maybe used
	//FrameID endFrame = element->mKeyFrame->mArray[0]->frameId;

	for (FrameID id = 0; id < element->mKeyFrame->mArraySize; ++id)
	{
	    RawKeyFrame* rawFrame = element->mKeyFrame->mArray[id];
	    Matrix4x4f qMat(1.0);

	    Quat firstQuat = {0.0f, 0.0f, 0.0f, 1.0f};

	    RawKeyFrame* prev = rawFrame->prevRawFrame;

	    // firstFrame commented out, but left in case maybe used
	    //FrameID firstFrame = element->mPrevoius->mKeyFrame->mArray[0]->frameId;
	    if (prev != NULL)
	    {
	        memcpy(firstQuat, prev->rotQuat, sizeof(firstQuat));

	        // firstFrame commented out, but left in case maybe used
	        //firstFrame = prev->frameId;
	    }
	    else
	    {
	        LOG_DET("prev is NULL");
	    }
	    Quat secondQuat;
	    memcpy(secondQuat, rawFrame->rotQuat, sizeof(secondQuat));

	    Matrix::quatInterpolateSlerp(qMat, firstQuat, secondQuat, factor);


	    Bone* b = mBoneList->mArray[rawFrame->boneId].get();
	    b->transform(qMat, b);
	}

	for (BoneID id = 0; id < mBoneList->mArraySize; ++id)
	{
		Matrix4x4f* p = (Matrix4x4f*)(pData + (id * SIZE_OF_MATRIX));
		*p = bl[id]->mFramePoseMatrix * bl[id]->mModificationMatrix * bl[id]->mInverseMatrix;
	}
}

uint32_t BoneKeyFrames::getFrameSize(void)
{
	return (MAX_NUM_OF_BONES * sizeof(Matrix4x4f));
}

static void fillBoneChildren(Array<Bone::Ptr>& pBones)
{
	LOG_INF("Filling bones with their children...");
	for (uint16_t boneIdx = 0; boneIdx < pBones.mArraySize; ++boneIdx)
	{
		std::list<Bone*> l;
		for (uint16_t searchIdx = 0; searchIdx < pBones.mArraySize; ++searchIdx)
		{
			if (boneIdx == searchIdx) continue;
			Bone* b = pBones.mArray[searchIdx].get();
			if (b->getParentIdx() == boneIdx)
			{
				l.push_back(b);
			}
		}
		Bone* b = pBones.mArray[boneIdx].get();
		Array<Bone*>* childrenList = new Array<Bone*>(l.size());
		uint16_t nextB = 0;
		for (std::list<Bone*>::iterator it = l.begin(); it != l.end(); it++, nextB++)
		{
			childrenList->mArray[nextB] = *it;
		}
		b->setChildren(childrenList);
	}
	LOG_INF("...DONE");
}

static void fillKeyFrameMap(Array<Bone::Ptr>& pBoneList,
							Array<KeyFrameMapElement>& pMap,
							Array<RawKeyFrame>& pKeyFrames,
							FrameID pFrameNum)
{
	typedef std::map<FrameID, std::list<RawKeyFrame*> > MapType;

	LOG_INF("Filling STL Key Frame Map...");
	MapType stl_map;
	for (FrameID id = 0; id < pKeyFrames.mArraySize; ++id)
	{
		RawKeyFrame* keyFrame = &pKeyFrames.mArray[id];
		LOG_ASSERT(keyFrame->frameId < pFrameNum,
				"key frame can't be greater that frames num (" <<
				keyFrame->frameId << "/" << pFrameNum << ")");
		stl_map[keyFrame->frameId].push_back(keyFrame);
	}
	LOG_INF("...DONE");

	MapType::iterator stl_map_it = stl_map.begin();
	MapType::iterator stl_map_it_end = stl_map.end();
	FrameID beginFrameID = 0;
	FrameID prevFrameID = (--stl_map_it_end)->first;
	LOG_INF("Rewriting Key Frames from STL Map to map...");
	do
	{
		FrameID nextFrame = stl_map_it->first;
		for (FrameID mapID = beginFrameID; mapID < nextFrame; ++mapID)
		{
			pMap.mArray[mapID].mNext = &pMap.mArray[nextFrame];
			pMap.mArray[mapID].mPrevoius = &pMap.mArray[prevFrameID];
		}
		uint16_t rawKeysNum = stl_map_it->second.size();
		LOG_DEB("rawKeysNum: " << rawKeysNum);
		pMap.mArray[nextFrame].mKeyFrame.reset(new Array<RawKeyFrame*>(rawKeysNum));
		pMap.mArray[nextFrame].mPrevoius = &pMap.mArray[prevFrameID];

		std::list<RawKeyFrame*>& l = stl_map_it->second;
		std::list<RawKeyFrame*>::iterator lIt = l.begin();
		FrameID rawIdx = 0;
		do
		{
			RawKeyFrame* rkf = *lIt;
			pMap.mArray[nextFrame].mKeyFrame.get()->mArray[rawIdx] = rkf;
			rkf->prevRawFrame = findPrevKeyFrame(rkf->boneId, stl_map[prevFrameID]);
			LOG_DEB("raw frame, ptr: " << rkf << ", bone id: " << rkf->boneId << ", frame id: " <<
					rkf->frameId << ", prev raw frame: " << rkf->prevRawFrame);
			++rawIdx;
		}
		while (++lIt != l.end());

		sortRawKeyFrames(*pMap.mArray[nextFrame].mKeyFrame.get());

		beginFrameID = nextFrame + 1;
		prevFrameID = nextFrame;
	}
	while ((++stl_map_it) != stl_map.end());
	LOG_INF("...DONE");

	// last key frame must be the last frame!
	uint16_t lastKeyFrame = (--stl_map.end())->first;
	LOG_ASSERT( uint16_t(lastKeyFrame + 1) == pMap.mArraySize,
			"the last frame must be key frame (" << lastKeyFrame << "/" << pMap.mArraySize << ")");
}

static void sortRawKeyFrames(Array<RawKeyFrame*>& array)
{
#if 0
	LOG_INF("Raw Key Frames sorting:");
	for (FrameID id = 0; id < array.mArraySize; ++id)
	{
		LOG_INF("id: " << id << ", boneId: " << array.mArray[id]->boneId);
	}
#endif

	::qsort(array.mArray, array.mArraySize, sizeof(RawKeyFrame*), RawKeyFrameCMP);

#if 0
	for (FrameID id = 0; id < array.mArraySize; ++id)
	{
		LOG_INF("id: " << id << ", boneId: " << array.mArray[id]->boneId);
	}
#endif
}

static int RawKeyFrameCMP(const void * a, const void * b)
{
	RawKeyFrame* first = *((RawKeyFrame**)a);
	RawKeyFrame* second = *((RawKeyFrame**)b);

	return second->boneId - first->boneId;
}

static RawKeyFrame* findPrevKeyFrame(BoneID bId, std::list<RawKeyFrame*>& list)
{
	std::list<RawKeyFrame*>::iterator it = list.begin();
	do
	{
		if ((*it)->boneId == bId)
		{
			return *it;
		}
	}
	while (++it != list.end());

	return NULL;
}
