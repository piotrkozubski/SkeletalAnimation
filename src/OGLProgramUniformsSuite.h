/*
 * OGLProgramUniformsSuite.h
 *
 *  Created on: 15.07.2017
 *      Author: Piotr Kozubski
 */

#ifndef OGLPROGRAMUNIFORMSSUITE_H_
#define OGLPROGRAMUNIFORMSSUITE_H_

#include "Array.h"
#include "OGLProgramUniform.h"

/**
 * OpenGL Program Uniforms Suite class
 *
 * OGLProgramUniformsSuite class implements uniforms suite.
 */
class OGLProgramUniformsSuite : public Array<OGLProgramUniform>
{
private:
	/**
	 * Disabled constructors
	 */
	OGLProgramUniformsSuite();
	OGLProgramUniformsSuite(const OGLProgramUniformsSuite& s);

public:
	/**
	 * Constructor
	 */
	OGLProgramUniformsSuite(uint32_t pSize) : Array(pSize) { };

	/**
	 * Destructor
	 */
	~OGLProgramUniformsSuite() { };

	/**
	 * Assigns the data value to the specific uniform by given uniform name.
	 * If uniform name is not found in the array, it returns 'false', otherwise,
	 * it return 'true'.
	 */
	bool set(UniformName pUnfName, const void* data);
};

/**
 * OGL uniform suite validator function
 *
 * @param suite			reference to uniform suite to be validated
 * @param suiteName		pointer to suite name string
 */
void validateOGLUniformSuite(const OGLProgramUniformsSuite& suite,
							 const char* suiteName);

#endif /* OGLPROGRAMUNIFORMSSUITE_H_ */
