
#ifndef BONE_FRAMES_
#define BONE_FRAMES_

#include <auto_ptr.h>
#include "Array.h"

/**
 * BonesFrame class
 * BonesFrame returns bones' transformation matrixes for time
 */
class BonesFrame
{
public:

	/**
	 *  Destructor
	 */
	~BonesFrame() {}

	/**
	 * typedef of smart pointer to BonesFrame
	 */
	typedef std::auto_ptr<BonesFrame> Ptr;

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
	void getFrame(Array<uint8_t>& pData, uint16_t pStart, uint16_t pEnd,
				  float pFactor);

	/**
	 * Returns size of one frame for all bones matrixes
	 */
	uint32_t getFrameSize(void);

private:
	Array<uint8_t>::Ptr mData;

	/**
	 * Constructor.
	 * Constructs object. pData will be deallocated when object is dead.
	 *
	 * @param pData		data of bones' key frames
	 */
	BonesFrame(Array<uint8_t>* pData);
};

#endif
