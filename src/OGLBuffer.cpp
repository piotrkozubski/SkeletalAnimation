/*
 * OGLBuffer.cpp
 *
 *  Created on: 29.06.2019
 *      Author: zokp
 */
#include <cstring>
#include "OGLBuffer.h"
#include "Logger.h"
#include "OGLFunctions.h"

OGLBuffer* OGLBuffer::create(uint8_t* pData, uint32_t pSize)
{
	LOG_ASSERT(pData, "pData not set");
	LOG_ASSERT(pSize, "pSize not set");

	GLuint ssb  = 0;

	glGenBuffers(1, &ssb);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssb);
	glBufferData(GL_SHADER_STORAGE_BUFFER, pSize, pData, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return new OGLBuffer(ssb, pSize);
}

OGLBuffer::OGLBuffer(GLuint pSSB, uint32_t pSize) :
				mSSB(pSSB),
				mSize(pSize)
{
	LOG_ASSERT(mSSB, "mSSB not set");
	LOG_ASSERT(mSize, "mSize not set");
}

void OGLBuffer::getData(uint8_t* pData)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mSSB);
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	memcpy(pData, p, mSize);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void OGLBuffer::putData(uint8_t* pData)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mSSB);
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	memcpy(p, pData, mSize);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}
