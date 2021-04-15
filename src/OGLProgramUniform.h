/*
 * OGLProgramUniform.h
 *
 *  Created on: May 2, 2013
 *      Author: pkozubsk
 */

#ifndef OGLPROGRAMUNIFORM_H_
#define OGLPROGRAMUNIFORM_H_

#include <GL/glcorearb.h>
#include <cstddef>

/**
 *  Enum type defining possible program uniform names.
 */
enum UniformName
{
	UNFNAME_NONE = 0,
	UNFNAME_MDVPROJMAT,
	UNFNAME_MODELMAT,
	UNFNAME_TEXTUNIT,
	UNFNAME_MESHCOLOR,
	UNFNAME_BONES,
	UNFNAME_NUM
};

/**
 *  OGLProgramUniform structure.
 *
 *  Holds information about program uniform.
 */
struct OGLProgramUniform
{
	OGLProgramUniform() :
		name(NULL),
		enumName(UNFNAME_NONE),
		data(NULL),
		type(UNIFORMTYPE_NONE),
		location(-1)
	{}

    /**
     *  Enum type defining possible program uniform types.
     */
    enum UniformType
    {
    	UNIFORMTYPE_NONE = 0,
        UNIFORMTYPE_VEC3F,
        UNIFORMTYPE_VEC4F,
		UNIFORMTYPE_FLOAT,
		UNIFORMTYPE_INT,
		UNIFORMTYPE_MAT4F,
        UNIFORMTYPE_NUM
    };

    const char* name;
	UniformName enumName;
    void*       data;
    UniformType type;
    GLint       location;
    int			count;

};

#endif /* OGLPROGRAMUNIFORM_H_ */
