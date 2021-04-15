/*
 * OGLUtils.h
 *
 *  Created on: Apr 16, 2013
 *      Author: pkozubsk
 */

#ifndef OGLUTILS_H_
#define OGLUTILS_H_

/**
 * OpenGL utilities name-space
 *
 * OGLUtils name-space should contain non-member functions which implement
 * separate algorithms using OpenGL calls.
 *
 */
namespace OGLUtils
{
    /**
     * Initiates OpenGL environment
     */
    void initOGL(void);

    /**
     *  Implements OpenGL operations on window's reshape event.
     *
     *  @param pWidth   new width of the window
     *  @param pHeight  new height of the window
     */
    void reshapeOGLWindow(int pWidth, int pHeight);

    /**
     * Clear buffers (e.g. color, depth) for new frame rendering
     */
    void clearOGLBuffers(void);

    /**
     * Enable/Disable wireframe rendering mode
     * @param flag		enable/disable
     */
    void enableWireframeMode(bool flag);

    /**
     * Returns viewport coordinates and resolution
     */
    void getViewport(int &x, int &y, int &width, int &height);

    /**
     * Prints OpenGL parameters
     */
    void printParams(void);
}


#endif /* OGLUTILS_H_ */
