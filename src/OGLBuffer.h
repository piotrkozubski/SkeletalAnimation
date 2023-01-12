/*
 * OGLBuffer.h
 *
 *  Created on: 29.06.2019
 *      Author: zokp
 */

#ifndef OGLBUFFER_H_
#define OGLBUFFER_H_

#include <GL/glcorearb.h>

/**
 * Class responsible for OGLBuffer object creation
 */
class OGLBuffer
{
public:

	/**
	 *	Creates OGLBuffer object
	 *
	 *	@param pData	pointer to data for buffer
	 *	@param pSize	size of data for buffer
	 *
	 *	@return 		pointer to OGLBuffer object
	 */
	static OGLBuffer* create(uint8_t* pData, uint32_t pSize);

	/**
	 *	Returns buffer size
	 */
	uint32_t getSize(void);

	/**
	 * Returns pointer to data mapped to buffer
	 *
	 * @param pData		pointer to data to be copied.
	 * 					Assumes pData has proper size allocated.
	 */
	void getData(uint8_t* pData);

	/**
	 * Copies data into the buffer
	 *
	 * @param pData		pointer to data to be copied.
	 * 					Assumes pData has proper size allocated.
	 */
	void putData(uint8_t* pData);

private:

	/**
	 * Object constructor
	 *
	 * @param	pSSB	descriptor
	 * @param 	pSize	size of buffer
	 *
	 */
	OGLBuffer(GLuint pSSB, uint32_t pSize);

	GLuint 		mSSB;
	uint32_t	mSize;
};

inline uint32_t OGLBuffer::getSize(void)
{
	return mSize;
}

#endif /* OGLBUFFER_H_ */
