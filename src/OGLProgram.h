/*
 * OGLProgram.h
 *
 *  Created on: Apr 29, 2013
 *      Author: pkozubsk
 */

#ifndef OGLPROGRAM_H_
#define OGLPROGRAM_H_

#include <memory>

#include "OGLShader.h"
#include "OGLProgramUniform.h"
#include "OGLProgramUniformsSuite.h"
#include "OGLProgramAttrib.h"

/**
 * OpenGL Program class
 *
 * OGLProgram class contains interface to manage OpenGL program object
 */
class OGLProgram
{
private:
    /**
     *  Private OGLProgram constructor
     *
     *  @param  shader  shader's identifier in OpenGL
     */
    OGLProgram(GLuint program);

public:

    /**
     *  OGLProgram destructor - removes program from OpenGL
     *  (as soon as not attached to any other object)
     */
    ~OGLProgram();

    /**
     *  Static factory method. Creates OGLProgram object attaching given
     *  vertex and fragment shaders.
     *  attrib is used to set proper attribute's location in the shaders.
     *
     *  @param  vertShader  reference to vertex shader
     *  @param  fragShader  reference to fragment shader
     *  @param  attribCount attribute count
     *  @param  attrib      reference to attributes
     *  @return             pointer to OGLProgram object, or NULL if failed
     */
    static OGLProgram* create(const OGLShader&          vertShader,
            				  const OGLShader&          fragShader,
							  uint32_t                  attribCount,
							  const OGLProgramAttrib*   attrib);

    /**
     *  Makes the program active and sets given parameters (GLSL uniforms).
     *  Shaders attached to this program will be executed next time when
     *  rendering pipeline is processing data.
     *
     *  @param  count       number of parameters
     *  @param  param       reference to uniforms suite
     */
    void use(const OGLProgramUniformsSuite& uniform) const;

    /**
     * Gets attribute's location in program
     */
    void getAttribLocation(OGLProgramAttrib& attrib) const;

    /**
     *  Returns param's location for given name.
     *
     *  @param  param       reference to parameter structure with legal name,
     *                      where location will be stored.
     *
     */
    void getUniformLocation(OGLProgramUniform& uniform) const;

    /**
     *  typedef of smart pointer to OGLProgram
     */
    typedef std::auto_ptr<OGLProgram> Ptr;

private:

    /**
     *  Loads parameter into OpenGL GLSL program.
     *
     *  @param  param       reference to OGLProgramParam structure
     */
    void loadUniform(const OGLProgramUniform& uniform) const;

    /**
     *  Returns param's location for given name.
     *
     *  @param  name	name of the uniform that location is returned
     *
     */
    GLint getUniformLocation(const char* name) const;

private:
    GLuint mProgramId;
};

#endif /* OGLPROGRAM_H_ */
