/*
 * BMP.cpp
 *
 *  Created on: 28.06.2017
 *      Author: Piotr Kozubski
 */
#include <istream>
#include <fstream>

#include "BMP.h"
#include "Logger.h"

using namespace std;

const uint8_t BITMAPV5HEADER_SIZE = 124;

//supported BMP parameters
const uint32_t SUPPORTED_RED_MASK = 		0xFF000000;
const uint32_t SUPPORTED_GREEN_MASK = 		0x00FF0000;
const uint32_t SUPPORTED_BLUE_MASK = 		0x0000FF00;
const uint32_t SUPPORTED_ALPHA_MASK = 		0x000000FF;
const uint32_t SUPPORTED_BPP = 				32;
const uint32_t SUPPORTED_COMPRESSION_TYPE = 0x3;

/**
 *  BMP header structure
 *
 *  Supports BITMAP V5 Header
 */
struct BMPHeader { // not complete BMP header, because we don't need all
	struct bitmapHeader {
		uint16_t magic;
		uint32_t fileSize;
		uint32_t reserved;
		uint32_t pixelOffset;
	} __attribute__((packed)) bitmapHeader;
	struct DIBHeader {
		uint32_t DIBHeaderSize;
		uint32_t imageWidth;
		uint32_t imageHeight;
		uint16_t planes;
		uint16_t bitsPerPixel;
		uint32_t compression;
		uint32_t imageSize;
		uint32_t xPixelsPerMeter;
		uint32_t yPixelsPerMeter;
		uint32_t colorsInColorTable;
		uint32_t importantColorCount;
		uint32_t redMask;
		uint32_t greenMask;
		uint32_t blueMask;
		uint32_t alphaMask;
		// the rest is unsupported yet
	} __attribute__((packed)) dibHeader;
} __attribute__((packed));

static void logHeader(const BMPHeader &h);
static uint8_t calcBMPPadding(uint32_t width);
static bool validateBMPHeader(const BMPHeader &h);

uint32_t BMP::readBMPFile(
			const char* pBmpFileName,
			std::unique_ptr<uint8_t>& pPixelsData,
			uint32_t& pWidth,
			uint32_t& pHeight,
			bool& pIsAlpha)
{
	LOG_INF("loading BMP file " << pBmpFileName);

	pWidth = 0;
	pHeight = 0;
	pIsAlpha = false;

	BMPHeader header;
    ifstream file;
    file.open(pBmpFileName, ios::out | ios::binary);
    file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader)); // reading with litte-endian on x86

    logHeader(header);
    if (!validateBMPHeader(header))
    {
    	return 0;
    }

    pPixelsData.reset(new uint8_t[header.dibHeader.imageSize]);
    uint32_t pixelSize = (header.dibHeader.bitsPerPixel >> 3);
    uint32_t rowSize = (header.dibHeader.imageWidth * pixelSize);
    uint8_t padding = calcBMPPadding(rowSize);

    for (uint32_t row = 0; row < header.dibHeader.imageHeight; ++row)
    {
    	uint32_t rowOff = (row * (rowSize + padding));
    	file.seekg(header.bitmapHeader.pixelOffset + rowOff);
    	file.read(reinterpret_cast<char*>(pPixelsData.get()) + rowOff, rowSize);
    	//char *dst = reinterpret_cast<char*>(pPixelsData.get());
    	//BMP is saved first row of the file is the bottom row of the image
    	//file.read((dst + header.dibHeader.imageSize - rowSize) - rowOff, rowSize);
    }

    pWidth = header.dibHeader.imageWidth;
    pHeight = header.dibHeader.imageHeight;
    pIsAlpha = (header.dibHeader.alphaMask != 0);

    return header.dibHeader.imageSize;
}

static uint8_t calcBMPPadding(uint32_t width)
{
	uint8_t mod = (width % 4);
	uint8_t padding = 0;
	if (mod > 0)
	{
		padding = 4 - mod;
	}
	return padding;
}

static void logHeader(const BMPHeader &h)
{
    LOG_DEB("bmp header:");
    LOG_DEB(showbase << "magic: " << hex << h.bitmapHeader.magic);
    LOG_DEB(showbase << "file size: " << h.bitmapHeader.fileSize);
    LOG_DEB(showbase << "reserved: " << hex << h.bitmapHeader.reserved);
    LOG_DEB(showbase << "pixel offset: " << hex << h.bitmapHeader.pixelOffset);
    LOG_DEB(showbase << "DIB header size: " << h.dibHeader.DIBHeaderSize);
    LOG_DEB(showbase << "image width: " << h.dibHeader.imageWidth);
    LOG_DEB(showbase << "image height: " << h.dibHeader.imageHeight);
    LOG_DEB(showbase << "planes: " << hex << h.dibHeader.planes);
    LOG_DEB(showbase << "bits per pixel: " << h.dibHeader.bitsPerPixel);
    LOG_DEB(showbase << "compression: " << hex << h.dibHeader.compression);
    LOG_DEB(showbase << "image size: " << h.dibHeader.imageSize);
    LOG_DEB(showbase << "xPixelsPerMeter: " << h.dibHeader.xPixelsPerMeter);
    LOG_DEB(showbase << "yPixelsPerMeter: " << h.dibHeader.yPixelsPerMeter);
    LOG_DEB(showbase << "colorsInColorTable: " << h.dibHeader.colorsInColorTable);
    LOG_DEB(showbase << "importantColorCount: " << h.dibHeader.importantColorCount);
    LOG_DEB(showbase << "redMask: " << hex << h.dibHeader.redMask);
    LOG_DEB(showbase << "greenMask: " << hex << h.dibHeader.greenMask);
    LOG_DEB(showbase << "blueMask: " << hex << h.dibHeader.blueMask);
    LOG_DEB(showbase << "alphaMask: " << hex << h.dibHeader.alphaMask);
}

static bool validateBMPHeader(const BMPHeader &h)
{
	if (h.dibHeader.DIBHeaderSize != BITMAPV5HEADER_SIZE)
	{
		LOG_ERR("BMP NOT SUPPORTED. BMP header type not supported, size: " <<
				h.dibHeader.DIBHeaderSize);
		return false;
	}
	if (h.dibHeader.compression != SUPPORTED_COMPRESSION_TYPE)
	{
		LOG_ERR("BMP NOT SUPPORTED. Only BMP compression " << SUPPORTED_COMPRESSION_TYPE
				<< " is supported");
		return false;
	}
	if (h.dibHeader.bitsPerPixel != SUPPORTED_BPP)
	{
		LOG_ERR("BMP NOT SUPPORTED. Bits per pixel must be " << SUPPORTED_BPP);
		return false;
	}
	if (h.dibHeader.redMask != SUPPORTED_RED_MASK)
	{
		LOG_ERR("BMP NOT SUPPORTED. Red mask must be " << hex << SUPPORTED_RED_MASK);
		return false;
	}
	if (h.dibHeader.greenMask != SUPPORTED_GREEN_MASK)
	{
		LOG_ERR("BMP NOT SUPPORTED. Green mask must be " << hex << SUPPORTED_GREEN_MASK);
		return false;
	}
	if (h.dibHeader.blueMask != SUPPORTED_BLUE_MASK)
	{
		LOG_ERR("BMP NOT SUPPORTED. Blue mask must be " << hex << SUPPORTED_BLUE_MASK);
		return false;
	}
	if (h.dibHeader.alphaMask > 0 && h.dibHeader.alphaMask != SUPPORTED_ALPHA_MASK)
	{
		LOG_ERR("BMP NOT SUPPORTED. Alpha mask must be " << hex << SUPPORTED_ALPHA_MASK);
		return false;
	}
	return true;
}
