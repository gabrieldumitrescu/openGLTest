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
#include "lighting_technique.h"
#include "glut_backend.h"
#include "math_3d.h"
#include "Pipeline.h"
#include "camera.h"

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 800

struct Vertex{
    Vector3f pos;
    Vector2f texCoord;
    Vertex(){}
    Vertex(const Vector3f& position, const Vector2f& textureCoord):
    pos(position),texCoord(textureCoord){}
    
};

class OpenGLTest: public ICallbacks{
public:
    OpenGLTest(){
        pTexture=NULL;
        pGameCamera=NULL;
        pEffect=NULL;
        scale=0.0f;
        directionalLight.ambientIntensity=0.1;
        directionalLight.color=Vector3f(1.0f,1.0f,1.0f);        
    }
    
    bool Init(){
        pGameCamera=new Camera(WINDOW_WIDTH,WINDOW_HEIGHT);
        CreateVertexBuffer();
        CreateIndexBuffer();
        pEffect=new LightingTechnique();
        if(!pEffect->Init()) return false;
        pEffect->Enable();
        pEffect->setTextureUnit(0);
        
        pTexture = new Texture(GL_TEXTURE_2D, "test.png");
        if (!pTexture->load()) {
            return false;
        }
        return true;
    }
    
    void Run(){
        GLUTBackendRun(this);
    }
    
    virtual void renderSceneCB(){
        pGameCamera->onRender();
  
        glClear(GL_COLOR_BUFFER_BIT);

        scale+=0.1f;
        Pipeline p;
        //p.setScale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
        p.setTranslation(0.0f, 0.0f, 3.0f);
        p.setRotation(0.0f,scale,0.0f);
        p.setPersProj(60.0f,WINDOW_WIDTH,WINDOW_HEIGHT,1.0f,100.0f);
        p.setCamera(pGameCamera->getPosition(),pGameCamera->getTarget(), pGameCamera->getUp());
        
        pEffect->setWVP(p.getTransform());
        pEffect->setDirectionalLight(directionalLight);

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
    
    virtual void idleCB(){
        renderSceneCB();
    }
    
    virtual void keyboardCB(unsigned char Key,int x, int y){
        switch(Key){
        case 'q':
            glutLeaveMainLoop();
            break;
        case 'a':
            directionalLight.ambientIntensity+=0.05f;
            break;
        case 's':
            directionalLight.ambientIntensity-=0.05f;
            break;
        }
    }
    
    virtual void specialKeyboardCB(int Key, int x, int y){
        pGameCamera->onKeyboard(Key);
    }
    
    virtual void passiveMouseCB(int x, int y){
        pGameCamera->onMouse(x,y);
    }
    
    ~OpenGLTest(){
        delete pTexture;
        delete pGameCamera;
        delete pEffect;
    }
    
private:
    void CreateVertexBuffer(){
        Vertex vertices[4];
        vertices[0] = Vertex(Vector3f(-1.0f, -1.0f,  0.5773f),Vector2f(0.0f,0.0f));
        vertices[1] = Vertex(Vector3f( 0.0f, -1.0f, -1.15475f),Vector2f(0.563f,0.0f));
        vertices[2] = Vertex(Vector3f( 1.0f, -1.0f,  0.5773f),Vector2f(1.0f,0.0f));
        vertices[3] = Vertex(Vector3f( 0.0f,  1.0f,  0.0f),Vector2f(0.563f,1.0f));
        glGenBuffers(1,&VBObj);
        glBindBuffer(GL_ARRAY_BUFFER,VBObj);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    }

    void CreateIndexBuffer(){
        unsigned int indices[]={0, 3, 1,
                                1, 3, 2,
                                2, 3, 0,
                                0, 1, 2 };
        glGenBuffers(1,&IBObj);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBObj);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices,GL_STATIC_DRAW);
    }
    
        GLuint VBObj;
        GLuint IBObj;
        GLuint gSamplerLocation;
        GLuint gWVPLocation;
        float scale;
        LightingTechnique* pEffect;
        Texture* pTexture;
        Camera* pGameCamera;
        DirectionalLight directionalLight;
};


int main(int argc, char** argv){
    Magick::InitializeMagick(argv[0]);
    GLUTBackendInit(argc,argv);
    if(!GLUTBackendCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT,32,true,"OpenGL Test")){
        return 0;
    }
    
    OpenGLTest* program=new OpenGLTest();
    if(!program->Init()) return 1;
    
    program->Run();
    
    delete program;
    return 0;
}




