/*
 * OGLProgram.h
 *
 *  Created on: Apr 29, 2013
 *      Author: pkozubsk
 */
#include "OGLFunctions.h"

#include "OGLProgram.h"
#include "Logger.h"

OGLProgram::OGLProgram(GLuint programId) :
    mProgramId(programId)
{
    LOG_INF("OpenGL Program created (program id:" << mProgramId << ")");
}

OGLProgram::~OGLProgram()
{
    glDeleteProgram(mProgramId);

    LOG_INF("OpenGL Program deleted (program id:" << mProgramId << ")");
}

OGLProgram* OGLProgram::create(const OGLShader&          vertShader,
                               const OGLShader&          fragShader,
                               uint32_t                  attribCount,
                               const OGLProgramAttrib*   attrib)
{
    OGLProgram* programObj = NULL;

    GLuint programId = glCreateProgram();

    vertShader.attachToProgram(programId);
    fragShader.attachToProgram(programId);

    while (attribCount--)
    {
    	LOG_DEB("binding OGL attribute location with name: " << attrib[attribCount].location << attrib[attribCount].name);
        glBindAttribLocation(programId, attrib[attribCount].location,
                                        attrib[attribCount].name);
    }

    glLinkProgram(programId);

    GLint status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        LOG_ERR("error linking program");
        glDeleteProgram(programId);
    }
    else
    {
        programObj = new OGLProgram(programId);
    }

    return programObj;
}

void OGLProgram::use(const OGLProgramUniformsSuite& uniform) const
{
    glUseProgram(mProgramId);

    uint32_t count = uniform.mArraySize;
    while (count--)
    {
        loadUniform(uniform.mArray[count]);
    }
}

void OGLProgram::loadUniform(const OGLProgramUniform& uniform) const
{
    GLint loc = uniform.location;
    if (loc < 0)
    {
        loc = glGetUniformLocation(mProgramId, uniform.name);
    }

    switch (uniform.type)
    {
        case OGLProgramUniform::UNIFORMTYPE_VEC4F:
        {
            glUniform4fv(loc, uniform.count, static_cast<GLfloat*>(uniform.data));
        }
        break;
        case OGLProgramUniform::UNIFORMTYPE_VEC3F:
        {
        	glUniform3fv(loc, uniform.count, static_cast<GLfloat*>(uniform.data));
        }
        break;
        case OGLProgramUniform::UNIFORMTYPE_FLOAT:
        {
        	glUniform1fv(loc, uniform.count, static_cast<GLfloat*>(uniform.data));
        }
        break;
        case OGLProgramUniform::UNIFORMTYPE_INT:
        {
        	glUniform1iv(loc, uniform.count, static_cast<GLint*>(uniform.data));
        }
        break;
        case OGLProgramUniform::UNIFORMTYPE_MAT4F:
        {
        	glUniformMatrix4fv(loc, uniform.count, GL_FALSE, (static_cast<GLfloat*>(uniform.data)));
        }
        break;

        default:
        break;
    }
}

void OGLProgram::getUniformLocation(OGLProgramUniform& uniform) const
{
    uniform.location = glGetUniformLocation(mProgramId, uniform.name);
}

GLint OGLProgram::getUniformLocation(const char* name) const
{
    return glGetUniformLocation(mProgramId, name);
}

void OGLProgram::getAttribLocation(OGLProgramAttrib& attrib) const
{
    attrib.location = glGetAttribLocation(mProgramId, attrib.name);
}
