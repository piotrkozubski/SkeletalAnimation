/*
 * OGLProgramAttrib.h
 *
 *  Created on: May 2, 2013
 *      Author: pkozubsk
 */

#ifndef OGLPROGRAMATTRIB_H_
#define OGLPROGRAMATTRIB_H_

/**
 *  OGLProgramAttrib structure.
 *
 *  Holds information about GLSL program attribute.
 */
struct OGLProgramAttrib
{
    /**
     *  Fixed locations of program attributes'.
     */
    enum
    {
        ATTRIBIDX_VERTPOS = 0,
        ATTRIBIDX_VERTCOL,
        ATTRIBIDX_VERTNORM,
        ATTRIBIDX_VERTTEXCOORD,
		ATTRIBIDX_BONEIDXS,
		ATTRIBIDX_BONEWEIGHTS,
        ATTRIBIDX_NUM
    };

    const char* name;
    GLint       location;
};

#endif /* OGLPROGRAMATTRIB_H_ */
