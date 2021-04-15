/*
 * OGLUtils.cpp
 *
 *  Created on: Apr 16, 2013
 *      Author: pkozubsk
 */

#if WIN
#include <windows.h>
#endif

#include "OGLUtils.h"
#include "OGLFunctions.h"
#include "Logger.h"

void OGLUtils::initOGL(void)
{
#if WIN
    RegisterOGLFunctPointers();
#endif

    LOG_INF("Open GL vendor: " << glGetString(GL_VENDOR));
    LOG_INF("Open GL renderer: " << glGetString(GL_RENDERER));
    LOG_INF("Open GL version: " << glGetString(GL_VERSION));
    LOG_INF("Open GL GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0.2, 0.2, 0.2, 1);
    glDepthMask(GL_TRUE);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glLineWidth(4);
}

void OGLUtils::reshapeOGLWindow(int pWidth, int pHeight)
{
    glViewport(0, 0, pWidth, pHeight);
}

void OGLUtils::clearOGLBuffers(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OGLUtils::enableWireframeMode(bool flag)
{
	glPolygonMode(GL_FRONT_AND_BACK, flag ? GL_LINE : GL_FILL);
}

void OGLUtils::getViewport(int &x, int &y, int &width, int &height)
{
	int data[4];
	glGetIntegerv(GL_VIEWPORT, data);
	x = data[0];
	y = data[1];
	width = data[2];
	height = data[3];
}

void OGLUtils::printParams(void)
{
	GLint val = 0;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &val);
	LOG_INF("GL_MAX_UNIFORM_BLOCK_SIZE: " << val);
	val = 0;

	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &val);
	LOG_INF("GL_MAX_UNIFORM_BUFFER_BINDINGS: " << val);
	val = 0;

	glGetIntegerv(GL_UNIFORM_SIZE, &val);
	LOG_INF("GL_UNIFORM_SIZE: " << val);
	val = 0;

	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &val);
	LOG_INF("GL_MAX_VERTEX_UNIFORM_BLOCKS: " << val);
	val = 0;

	glGetIntegerv(GL_UNIFORM_BUFFER_SIZE, &val);
	LOG_INF("GL_UNIFORM_BUFFER_SIZE: " << val);
	val = 0;

	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &val);
	LOG_INF("GL_MAX_VERTEX_UNIFORM_VECTORS: " << val);
	val = 0;
}
