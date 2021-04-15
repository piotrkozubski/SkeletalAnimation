/*
 * OGLTexture.h
 *
 *  Created on: 25.06.2017
 *      Author: zokp
 */

#ifndef _OGLTEXTURE_H_
#define _OGLTEXTURE_H_

#include <GL/glcorearb.h>

/**
 * OpenGL Texture class
 *
 * OGLTexture class contains interface to manage OpenGL texture object
 */
class OGLTexture
{
private:
    /**
     *  Private OGLTexture constructor
     *
     *  @param  texture		texture's identifier in OpenGL
     */
	OGLTexture(GLuint texture);

public:

    /**
     *  OGLTexture destructor - removes texture from OpenGL
     *  (as soon as not attached to any other object)
     */
    ~OGLTexture();

    /**
     *  Public static OGLTexture constructor
     *
     *  @param  pPixelData		pointer to rgb image data
     */
	static OGLTexture* create(const uint8_t* pPixelData,
			   	   	   	   	  const uint32_t pWidth,
							  const uint32_t pHeight,
							  const bool     pIsAlpha);

	/**
	 *  Static member function responsible for OGLTexture object creation.
	 *
	 *  @param      Name of a BMP file.
	 *
	 *  @return     pointer to OGLTexture object
	 */
	static OGLTexture* createFromBMP(const char* pBmpFileName);

	/**
	 *  Makes the texture active in OpenGL
	 *
	 */
	void use(void) const;

	/**
	 * typedef of smart pointer to OGLTexture
	 */
	 typedef std::auto_ptr<OGLTexture> Ptr;
private:
	 /**
	  * ID of the texture
	  */
	 GLuint mTextureId;
};

#endif /* _OGLTEXTURE_H_ */
