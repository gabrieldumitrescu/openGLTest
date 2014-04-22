/* 
 * File:   main.cpp
 * Author: acordeon
 *
 * Created on April 7, 2014, 9:50 AM
 */

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "texture.h"
#include "util.h"
#include "math_3d.h"
#include "Pipeline.h"
#include "camera.h"

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

struct Vertex{
    Vector3f pos;
    Vector2f texCoord;
    Vertex(){}
    Vertex(const Vector3f& position, const Vector2f& textureCoord):
    pos(position),texCoord(textureCoord){}
    
};

GLuint VBObj;
GLuint IBObj;
GLuint gSamplerLocation;
GLuint gWVPLocation;
Texture* pTexture=NULL;

Camera* pGameCamera=NULL;

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

void displayCB();
void specialKeyboardCB(int,int,int);
void keyboardCB(unsigned char,int,int);
void passiveMouseCB(int,int);
void initGlutCallbacks();
void createVertexBuffer();
void createIndexBuffer();
void AddShader(GLuint, const char*, GLenum);
void CompileShaders();

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
  glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
  glutInitWindowPosition(100,100);

  if(!glutGet(GLUT_DISPLAY_MODE_POSSIBLE)){
    fprintf(stderr,"Could not start OpenGL.\n");
    return 1;
  }
  
  glutCreateWindow("Test openGL");
  glutGameModeString("1366x768@32");
  glutEnterGameMode();

  initGlutCallbacks();
  
  pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

  // Must be done after glut is initialized!
   GLenum res = glewInit();
   if (res != GLEW_OK) {
     fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
     return 1;
   }
   printf("GL version: %s\n", glGetString(GL_VERSION)); 

  
  glClearColor(0.0f,0.0f,0.0f,0.0f);
  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  createVertexBuffer();
  createIndexBuffer();
  CompileShaders(); 

  glUniform1i(gSamplerLocation, 0);
  pTexture = new Texture(GL_TEXTURE_2D, "test.png");
  if (!pTexture->load()) {
        return 1;
  }
  glutMainLoop();

  return 0;
}

void displayCB(){
    pGameCamera->onRender();
  
  glClear(GL_COLOR_BUFFER_BIT);


  static float Scale=0.0f;
  Scale+=0.1f;
  Pipeline p;
  //p.setScale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
  p.setTranslation(0.0f, 0.0f, 3.0f);
  p.setRotation(0.0f,Scale,0.0f);
  p.setPersProj(60.0f,WINDOW_WIDTH,WINDOW_HEIGHT,1.0f,100.0f);
  p.setCamera(pGameCamera->getPosition(),pGameCamera->getTarget(), pGameCamera->getUp());
  glUniformMatrix4fv(gWVPLocation,1,GL_TRUE,(const GLfloat*)p.getTransform());

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER,VBObj);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
  glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const GLvoid*)12);
  
  pTexture->bind(GL_TEXTURE0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBObj);
    
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glutSwapBuffers();

}

void specialKeyboardCB(int Key, int x, int y)
{
    pGameCamera->onKeyboard(Key);
    //fprintf(stderr,"Keypress with mouse at %d x %d \n", x, y);
}

void keyboardCB(unsigned char Key, int x, int y){
    switch(Key){
        case 'q':
            glutLeaveMainLoop();
            break;
    }
}

void passiveMouseCB(int x, int y){
    pGameCamera->onMouse(x,y);
}

void initGlutCallbacks(){
  glutDisplayFunc(displayCB);
  glutIdleFunc(displayCB);
  glutSpecialFunc(specialKeyboardCB);
  glutPassiveMotionFunc(passiveMouseCB);
  glutKeyboardFunc(keyboardCB);
  
}

void createVertexBuffer(){
  Vertex vertices[4];
  vertices[0] = Vertex(Vector3f(-1.0f, -1.0f,  0.5773f),Vector2f(0.0f,0.0f));
  vertices[1] = Vertex(Vector3f( 0.0f, -1.0f, -1.15475f),Vector2f(0.563f,0.0f));
  vertices[2] = Vertex(Vector3f( 1.0f, -1.0f,  0.5773f),Vector2f(1.0f,0.0f));
  vertices[3] = Vertex(Vector3f( 0.0f,  1.0f,  0.0f),Vector2f(0.563f,1.0f));
  glGenBuffers(1,&VBObj);
  glBindBuffer(GL_ARRAY_BUFFER,VBObj);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
}

void createIndexBuffer(){
    unsigned int indices[]={0, 3, 1,
                            1, 3, 2,
                            2, 3, 0,
                            0, 1, 2 };
    glGenBuffers(1,&IBObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices,GL_STATIC_DRAW);
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}


void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }
    
    std::string vs, fs;

    if (!ReadFile(pVSFileName, vs)) {
        exit(1);
    };

    if (!ReadFile(pFSFileName, fs)) {
        exit(1);
    };

   // printf("Vertex shader:%s\n",vs.c_str());
    //printf("Fragment shader:%s\n",fs.c_str());
    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
            glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);
    
    gWVPLocation=glGetUniformLocation(ShaderProgram,"gWVP");
    assert(gWVPLocation!=0xFFFFFFFF);
    
    gSamplerLocation=glGetUniformLocation(ShaderProgram,"gSampler");
    assert(gSamplerLocation!=0xFFFFFFFF);
    
}


