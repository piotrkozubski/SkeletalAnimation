/*
 * OGLfunctions.cpp
 *
 *  Created on: 29 mar 2015
 *      Author: zokp
 */
#include <stdlib.h>

#include "OGLFunctions.h"
#include "Logger.h"

#if WIN

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORM4FVPROC glUniform4fv = NULL;
PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLDRAWARRAYSPROC glDrawArrays = NULL;
PFNGLVIEWPORTPROC glViewport = NULL;
PFNGLGETSTRINGPROC glGetString = NULL;
PFNGLCLEARCOLORPROC glClearColor = NULL;
PFNGLCLEARPROC glClear = NULL;
PFNGLENABLEPROC glEnable = NULL;
PFNGLGETERRORPROC glGetError = NULL;
PFNGLDRAWELEMENTSPROC glDrawElements = NULL;
PFNGLPOLYGONMODEPROC glPolygonMode = NULL;
PFNGLGETINTEGERVPROC glGetIntegerv = NULL;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = NULL;
PFNGLDELETETEXTURESPROC glDeleteTextures = NULL;
PFNGLGENTEXTURESPROC glGenTextures = NULL;
PFNGLBINDTEXTUREPROC glBindTexture = NULL;
PFNGLTEXPARAMETERIPROC glTexParameteri = NULL;
PFNGLTEXIMAGE2DPROC glTexImage2D = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLCULLFACEPROC glCullFace = NULL;
PFNGLFRONTFACEPROC glFrontFace = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLDEPTHRANGEFPROC glDepthRange = NULL;
PFNGLDEPTHMASKPROC glDepthMask = NULL;
PFNGLDEPTHFUNCPROC glDepthFunc = NULL;
PFNGLLINEWIDTHPROC glLineWidth = NULL;
PFNGLUNIFORM1FVPROC glUniform1fv = NULL;
PFNGLUNIFORM1IVPROC glUniform1iv = NULL;
PFNGLMAPBUFFERPROC glMapBuffer = NULL;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = NULL;



static bool IsProcAddrBad(void (*p)())
{
    return (reinterpret_cast<int>(p) == 0 ||
           (reinterpret_cast<int>(p) == (0x1)) ||
           (reinterpret_cast<int>(p) == (0x2)) ||
           (reinterpret_cast<int>(p) == (0x3)) ||
           (reinterpret_cast<int>(p) == (-1)));
}

template <typename tFuncPtr>
static void GetAnyGLFuncAddress(tFuncPtr& rFunc, char const * name)
{
    rFunc = reinterpret_cast<tFuncPtr>(wglGetProcAddress(name));
    if (IsProcAddrBad(reinterpret_cast<void (*)()>(rFunc)))
    {
      HMODULE module = LoadLibraryA("opengl32.dll");
      rFunc = reinterpret_cast<tFuncPtr>(GetProcAddress(module, name));
    }
    LOG_ASSERT(rFunc, "Couldn't get OpenGL function address, name: " << name << ", rFunc: " << (int)rFunc);
}

void RegisterOGLFunctPointers(void)
{
    GetAnyGLFuncAddress(glGenVertexArrays, "glGenVertexArrays");
    GetAnyGLFuncAddress(glBindVertexArray, "glBindVertexArray");
    GetAnyGLFuncAddress(glGenBuffers, "glGenBuffers");
    GetAnyGLFuncAddress(glBindBuffer, "glBindBuffer");
    GetAnyGLFuncAddress(glBufferData, "glBufferData");
    GetAnyGLFuncAddress(glVertexAttribPointer, "glVertexAttribPointer");
    GetAnyGLFuncAddress(glEnableVertexAttribArray, "glEnableVertexAttribArray");
    GetAnyGLFuncAddress(glDeleteBuffers, "glDeleteBuffers");
    GetAnyGLFuncAddress(glDeleteVertexArrays, "glDeleteVertexArrays");
    GetAnyGLFuncAddress(glDeleteProgram, "glDeleteProgram");
    GetAnyGLFuncAddress(glCreateProgram, "glCreateProgram");
    GetAnyGLFuncAddress(glBindAttribLocation, "glBindAttribLocation");
    GetAnyGLFuncAddress(glLinkProgram, "glLinkProgram");
    GetAnyGLFuncAddress(glGetProgramiv, "glGetProgramiv");
    GetAnyGLFuncAddress(glUseProgram, "glUseProgram");
    GetAnyGLFuncAddress(glGetUniformLocation, "glGetUniformLocation");
    GetAnyGLFuncAddress(glUniform4fv, "glUniform4fv");
    GetAnyGLFuncAddress(glUniform3fv, "glUniform3fv");
    GetAnyGLFuncAddress(glUniform1f, "glUniform1f");
    GetAnyGLFuncAddress(glGetAttribLocation, "glGetAttribLocation");
    GetAnyGLFuncAddress(glDeleteShader, "glDeleteShader");
    GetAnyGLFuncAddress(glCreateShader, "glCreateShader");
    GetAnyGLFuncAddress(glShaderSource, "glShaderSource");
    GetAnyGLFuncAddress(glCompileShader, "glCompileShader");
    GetAnyGLFuncAddress(glGetShaderiv, "glGetShaderiv");
    GetAnyGLFuncAddress(glAttachShader, "glAttachShader");
    GetAnyGLFuncAddress(glDrawArrays, "glDrawArrays");
    GetAnyGLFuncAddress(glViewport, "glViewport");
    GetAnyGLFuncAddress(glGetString, "glGetString");
    GetAnyGLFuncAddress(glClearColor, "glClearColor");
    GetAnyGLFuncAddress(glClear, "glClear");
    GetAnyGLFuncAddress(glEnable, "glEnable");
    GetAnyGLFuncAddress(glGetError, "glGetError");
    GetAnyGLFuncAddress(glDrawElements, "glDrawElements");
    GetAnyGLFuncAddress(glPolygonMode, "glPolygonMode");
    GetAnyGLFuncAddress(glGetIntegerv, "glPolygonMode");
    GetAnyGLFuncAddress(glGenerateMipmap, "glGenerateMipmap");
    GetAnyGLFuncAddress(glDeleteTextures, "glDeleteTextures");
    GetAnyGLFuncAddress(glGenTextures, "glGenTextures");
    GetAnyGLFuncAddress(glBindTexture, "glBindTexture");
    GetAnyGLFuncAddress(glTexParameteri, "glTexParameteri");
    GetAnyGLFuncAddress(glTexImage2D, "glTexImage2D");
    GetAnyGLFuncAddress(glUniform1i, "glUniform1i");
    GetAnyGLFuncAddress(glBufferSubData, "glBufferSubData");
    GetAnyGLFuncAddress(glActiveTexture, "glActiveTexture");
    GetAnyGLFuncAddress(glCullFace, "glCullFace");
    GetAnyGLFuncAddress(glFrontFace, "glFrontFace");
	GetAnyGLFuncAddress(glUniformMatrix4fv, "glUniformMatrix4fv");
    GetAnyGLFuncAddress(glDepthRange, "glDepthRange");
    GetAnyGLFuncAddress(glDepthMask, "glDepthMask");
    GetAnyGLFuncAddress(glDepthFunc, "glDepthFunc");
    GetAnyGLFuncAddress(glLineWidth, "glLineWidth");
    GetAnyGLFuncAddress(glUniform1fv, "glUniform1fv");
    GetAnyGLFuncAddress(glUniform1iv, "glUniform1iv");
    GetAnyGLFuncAddress(glMapBuffer, "glMapBuffer");
    GetAnyGLFuncAddress(glUnmapBuffer, "glUnmapBuffer");
    GetAnyGLFuncAddress(glVertexAttribIPointer, "glVertexAttribIPointer");
}

#endif
