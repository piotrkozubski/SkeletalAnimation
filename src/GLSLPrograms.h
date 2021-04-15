/*
 * GLSLPrograms.h
 *
 *  Created on: Apr 30, 2013
 *      Author: pkozubsk
 */

#ifndef GLSLPROGRAMS_H_
#define GLSLPROGRAMS_H_

#include "OGLProgram.h"

class OGLProgramUniformsSuite;

/**
 * GLSL Programs name-space
 *
 * GLSLPrograms name-space should contain non-member functions which implements
 * creation of common GLSL shading programs.
 */
namespace GLSLPrograms
{
    /**
     *  GLSL Program names
     */
    enum GLSLProgName
    {
        GLSLPR_FIRST = 0,
        GLSLPR_PHONG_LIGHTING = GLSLPR_FIRST,
		GLSLPR_GUARD_LIGHTING,
		GLSLPR_COLOR,
		GLSLPR_PHONG_LIGHTING_BONE_ANIM,
        GLSLPR_NUM
    };

    /**
     * Increment operator for GLSLProgName enumeration type.
     */
    GLSLProgName operator++(GLSLProgName& t);

    /**
     *  Loads all GLSL programs into OpenGL.
     *
     *  @return pointer to array of OGLProgram::Ptr objects, GLSLPR_NUM length
     */
    const OGLProgram::Ptr* loadGLSLPrograms(void);

    /**
     * Returns uniforms suite for specific GLSL program.
     *
     * @param	pProgName	program enum name
     *
     * @return pointer to uniforms suite
     */
    OGLProgramUniformsSuite* getUniformsSuite(const GLSLProgName pProgName);
}

#endif /* GLSLPROGRAMS_H_ */
