
#ifndef BONE_KEY_FRAMES_
#define BONE_KEY_FRAMES_

#include <auto_ptr.h>
#include "Array.h"
#include "Bone.h"

const 	uint32_t NO_FRAME_ID = 0xFFFFFFFF;
typedef uint32_t FrameID;

struct RawKeyFrame
{
	BoneID		  boneId;
	FrameID		  frameId;
	float 		  posOffset[3];
	float 		  rotQuat[4];
	RawKeyFrame*  prevRawFrame;
};

struct KeyFrameMapElement
{
	Array<RawKeyFrame*>::Ptr  mKeyFrame;
	KeyFrameMapElement* 	  mPrevoius;
	KeyFrameMapElement* 	  mNext;

	KeyFrameMapElement() :
		mKeyFrame(),
		mPrevoius(NULL),
		mNext(NULL)
	{
	}
};

/**
 * BoneKeyFrame class
 * BoneKeyFrame returns bones' transformation matrixes for time
 */
class BoneKeyFrames
{
public:

	/**
	 *  Destructor
	 */
	~BoneKeyFrames();

	/**
	 * typedef of smart pointer to BonesFrame
	 */
	typedef std::auto_ptr<BoneKeyFrames> Ptr;

	/**
	 * Creates BoneKeyFrames object from bone list and raw key frames.
	 *
	 * @param pFramesNum 	number of frames
	 * @param pBoneList		smart pointer to array of smart pointers of bones
	 * @param pKeyFrames	reference to smart pointer to array of smart pointers of RawKeyFrames
	 *
	 * @return	pointer to BoneKeyFrames object
	 */
	static BoneKeyFrames* create(
			uint32_t pFramesNum, Array<Bone::Ptr>::Ptr pBoneList, Array<RawKeyFrame>::Ptr pKeyFrames);

	/**
	 * Returns bones' transformation matrixes for given factor.
	 * For pProgress = 1.0, pEnd frame is returned.
	 * For pProgress = 0.0, pStart frame is returned.
	 *
	 *	@param pData	byte array where data is returned
	 *					must be allocated for size returned by getFrameSize()
	 *	@param pStart	start frame
	 *	@param pEnd		end frame
	 *	@param pFactor 	factor of frames range
	 */
	void getFrame(uint8_t *pData, uint32_t pStart, uint32_t pEnd,
				  float pFactor);
	void getFrame(uint8_t *pData, float pFactor);

	/**
	 * Returns size of one frame for all bones matrixes
	 */
	uint32_t getFrameSize(void);

private:
	uint16_t						mFramesNum;
	Array<Bone::Ptr>::Ptr 			mBoneList;
	Array<RawKeyFrame>::Ptr			mKeyFrames;
	Array<KeyFrameMapElement>::Ptr	mMap;

	/**
	 * Constructor.
	 * Constructs object. pData will be deallocated when object is dead.
	 *
	 * @param pData		data of bones' key frames
	 */
	BoneKeyFrames(uint32_t 							pFramesNum,
				  Array<Bone::Ptr>::Ptr 			pBoneList,
				  Array<RawKeyFrame>::Ptr			pKeyFrames,
				  Array<KeyFrameMapElement>::Ptr	pMap);
};

#endif
