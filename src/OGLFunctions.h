/*
 * OGLfunctions.h
 *
 *  Created on: 29 mar 2015
 *      Author: zokp
 */

#ifndef OGLFUNCTIONS_H_
#define OGLFUNCTIONS_H_

#include <GL/glcorearb.h>

#if WIN
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDRAWARRAYSPROC glDrawArrays;
extern PFNGLVIEWPORTPROC glViewport;
extern PFNGLGETSTRINGPROC glGetString;
extern PFNGLCLEARCOLORPROC glClearColor;
extern PFNGLCLEARPROC glClear;
extern PFNGLENABLEPROC glEnable;
extern PFNGLGETERRORPROC glGetError;
extern PFNGLDRAWELEMENTSPROC glDrawElements;
extern PFNGLPOLYGONMODEPROC glPolygonMode;
extern PFNGLGETINTEGERVPROC glGetIntegerv;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLDELETETEXTURESPROC glDeleteTextures;
extern PFNGLGENTEXTURESPROC glGenTextures;
extern PFNGLBINDTEXTUREPROC glBindTexture;
extern PFNGLTEXPARAMETERIPROC glTexParameteri;
extern PFNGLTEXIMAGE2DPROC glTexImage2D;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLCULLFACEPROC glCullFace;
extern PFNGLFRONTFACEPROC glFrontFace;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLDEPTHRANGEFPROC glDepthRange;
extern PFNGLDEPTHMASKPROC glDepthMask;
extern PFNGLDEPTHFUNCPROC glDepthFunc;
extern PFNGLLINEWIDTHPROC glLineWidth;
extern PFNGLUNIFORM1FVPROC glUniform1fv;
extern PFNGLUNIFORM1IVPROC glUniform1iv;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;
extern PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;

void RegisterOGLFunctPointers(void);

#endif

#endif /* OGLFUNCTIONS_H_ */
