/*
 * OGLTexture.cpp
 *
 *  Created on: 25.06.2017
 *      Author: zokp
 */

/*
 * OGLTexture.h
 *
 *  Created on: 25.06.2017
 *      Author: zokp
 */
#include <cstdlib>
#include <memory>

#include "OGLTexture.h"
#include "OGLFunctions.h"

#include "BMP.h"
#include "Logger.h"

OGLTexture::OGLTexture(GLuint pTextureId) :
	mTextureId(pTextureId)
{
    LOG_INF("OpenGL Texture created (texture id:" << mTextureId << ")");
}

OGLTexture::~OGLTexture()
{
    glDeleteTextures(1, &mTextureId);

    LOG_INF("OpenGL Texture deleted (texture id:" << mTextureId << ")");
}

OGLTexture* OGLTexture::create(const uint8_t* pPixelData,
							   const uint32_t pWidth,
							   const uint32_t pHeight,
							   const bool     pIsAlpha)
{
	GLuint textureId;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, pIsAlpha ? GL_RGBA : GL_RGB,
				 pWidth, pHeight, 0, pIsAlpha ? GL_RGBA : GL_RGB,
				 GL_UNSIGNED_INT_8_8_8_8, pPixelData);

//	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return new OGLTexture(textureId);
}

OGLTexture* OGLTexture::createFromBMP(const char* pBmpFileName)
{
	std::auto_ptr<uint8_t> pixelData;
	uint32_t width;
	uint32_t height;
	bool isAlpha;

	if (!BMP::readBMPFile(pBmpFileName, pixelData, width, height, isAlpha))
	{
		LOG_ERR("Error reading BMP file");
		return NULL;
	}
	return OGLTexture::create(pixelData.get(), width, height, isAlpha);
}

void OGLTexture::use(void) const
{
	glBindTexture(GL_TEXTURE_2D, mTextureId);
}

