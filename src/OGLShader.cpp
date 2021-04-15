/*
 * OGLShader.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: pkozubsk
 */
#include <cstdio>

#include "OGLFunctions.h"
#include "OGLShader.h"
#include "Logger.h"


OGLShader::OGLShader(GLuint shaderId) :
    mShaderId(shaderId)
{
    LOG_INF("OpenGL Shader created (shader id:" << mShaderId << ")");
}

OGLShader::~OGLShader()
{
    glDeleteShader(mShaderId);

    LOG_INF("OpenGL Shader deleted (shader id:" << mShaderId << ")");
}

OGLShader* OGLShader::create(const char*      shaderSrc,
                                   uint32_t   srcLen,
                             const ShaderType type)
{
    LOG_ASSERT(shaderSrc && srcLen, "Shader source code is empty.")

    OGLShader* shaderObj = NULL;

    GLuint shader = glCreateShader(type);

    const GLint  *glSrcLen = reinterpret_cast<const GLint*>(&srcLen);
    glShaderSource(shader, 1, &shaderSrc, glSrcLen);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        LOG_ERR("error creating OpenGL shader, errcode=" << glGetError());
        glDeleteShader(shader);
    }
    else
    {
        shaderObj = new OGLShader(shader);
        LOG_ASSERT(shaderObj, "OGLShader returned NULL.");
    }

    return shaderObj;
}

void OGLShader::attachToProgram(GLuint programId) const
{
    glAttachShader(programId, mShaderId);
}
