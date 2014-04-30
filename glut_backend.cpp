/*File: glut_backend.cpp
 * GLUT setup functions.
 */

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glut_backend.h"


void GLUTBackendInit(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    
}

bool GLUTBackendCreateWindow(unsigned int width, 
                             unsigned int height,
                             unsigned int bpp,
                             bool isFullScreen,
                             const char* pTitle){
    if(!glutGet(GLUT_DISPLAY_MODE_POSSIBLE)){
        fprintf(stderr,"Could not start OpenGL.\n");
        return false;
    }
    if(isFullScreen){
        char modeString[64]={ 0 };
        snprintf(modeString,sizeof(modeString),"%dx%d@%d",width,height,bpp);
        glutGameModeString(modeString);
        glutEnterGameMode();
    }
    else{
        glutInitWindowSize(width,height);
        glutCreateWindow(pTitle);
    }
    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return false;
    }
    return true;
    
}

static ICallbacks* s_pCallbacks=NULL;

static void displayCB(){
    s_pCallbacks->renderSceneCB();
}

static void idleCB(){
    s_pCallbacks->idleCB();
}

static void specialKeyboardCB(int Key,int x, int y){
    s_pCallbacks->specialKeyboardCB(Key,x,y);
}

static void keyboardCB(unsigned char Key, int x, int y){
    s_pCallbacks->keyboardCB(Key,x,y);
}

static void passiveMouseCB(int x, int y){
    s_pCallbacks->passiveMouseCB(x,y);
}

static void InitCallbacks(){
    glutDisplayFunc(displayCB);
    glutIdleFunc(idleCB);
    glutSpecialFunc(specialKeyboardCB);
    glutPassiveMotionFunc(passiveMouseCB);
    glutKeyboardFunc(keyboardCB);
}

void GLUTBackendRun(ICallbacks* pCallbacks){
    if(!pCallbacks){
        fprintf(stderr,"%s : Callbacks not specified.\n",__FUNCTION__);
        return;
    }
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    s_pCallbacks=pCallbacks;
    InitCallbacks();
    
    glutMainLoop();
    
    
}