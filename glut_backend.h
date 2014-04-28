/* 
 * File:   glut_backend.h
 * Author: quentin
 *
 * Created on April 28, 2014, 7:13 PM
 */

#ifndef GLUT_BACKEND_H
#define	GLUT_BACKEND_H

#include "callbacks.h"

void GLUTBackendInit(int argc, char** argv);

bool GLUTBackendCreateWindow(unsigned int width, 
                             unsigned int height,
                             unsigned int bpp,
                             bool isFullScreen,
                             const char* pTitle);

void GLUTBackendRun(ICallbacks* pCallbacks);


#endif	/* GLUT_BACKEND_H */

