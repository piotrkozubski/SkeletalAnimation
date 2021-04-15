/*
 * BMP.h
 *
 *  Created on: 28.06.2017
 *      Author: zokp
 */

#ifndef BMP_H_
#define BMP_H_

#include <stdint.h>
#include <memory>

namespace BMP {

/**
 * Function reads BMP file and returns pixels in pPixelData buffer
 * It assumes the BMP is 32 BPP, RGB or RGBA, not compressed
 * Pixels are RGB or RGBA formatted.
 *
 * pPixelData		buffer to store pixel data
 * pWidth			width of the image
 * pHeight			height of the image
 * pIsAlpha			true if image contains alpha channel per pixel
 * return uint32_t	size of the image
 */
uint32_t readBMPFile(
		const char* pBmpFileName,
		std::auto_ptr<uint8_t>& pPixelsData,
		uint32_t& pWidth,
		uint32_t& pHeight,
		bool& pIsAlpha);
}

#endif /* BMP_H_ */
