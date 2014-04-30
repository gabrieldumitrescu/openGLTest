/* 
 * File:   technique.h
 * Author: quentin
 *
 * Created on April 24, 2014, 8:38 PM
 */

#ifndef TECHNIQUE_H
#define	TECHNIQUE_H


#include <list>
#include <GL/glew.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

class Technique{
public:
    Technique();
    virtual ~Technique();
    virtual bool Init();
    void Enable();
protected:
    bool AddShader(GLenum shaderType,const char* fileName);
    bool Finalize();
    GLint GetUniformLocation(const char* uniformName);
    GLint GetProgramParam(GLint param);
    GLuint shaderProgram;
public:
    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList shaderObjectList;
    
};



#endif	/* TECHNIQUE_H */

