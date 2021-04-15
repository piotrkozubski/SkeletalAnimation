/*
 * OGLShader.h
 *
 *  Created on: Apr 29, 2013
 *      Author: pkozubsk
 */

#ifndef OGLSHADER_H_
#define OGLSHADER_H_

#include <memory>
#include <GL/glcorearb.h>

/**
 * OpenGL Shader class
 *
 * OGLShader class contains interface to manage OpenGL shaders
 */
class OGLShader
{

private:
    /**
     *  Private OGLShader constructor
     *
     *  @param  shader  shader's identifier in OpenGL
     */
    OGLShader(GLuint shaderId);

public:

    /**
     *  Type of the shader
     */
    enum ShaderType
    {
        SHADER_VERTEX = GL_VERTEX_SHADER,
        SHADER_FRAGMENT = GL_FRAGMENT_SHADER
    };

    /**
     *  OGLShader destructor - removes shader from OpenGL
     *  (as soon as not attached to any other object)
     */
    ~OGLShader();

    /**
     *  Static factory method. Creates OGLShader object from a given GLSL source
     *  code and shader type.
     *
     *  @param  shaderSrc   pointer to characters describing GLSL code
     *  @param  srcLen      length of GLSL code (in bytes)
     *  @param  type        type of the shader
     *  @return             pointer to OGLShader object, or NULL if failed
     */
    static OGLShader* create(const char*      shaderSrc,
                                   uint32_t   srcLen,
                             const ShaderType type);

    /**
     *  Attaches shader to the OpenGL program
     *
     *  @param programId        id of program to which shader will be attached
     */
    void attachToProgram(GLuint programId) const;

    /**
     *  typedef of smart pointer to OGLShader
     */
    typedef std::auto_ptr<OGLShader> Ptr;

private:
    GLuint mShaderId;
};

#endif /* OGLSHADER_H_ */
