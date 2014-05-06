/* 
 * File:   technique.cpp
 * Author: quentin
 */

#include "technique.h"


Technique::Technique(){
    shaderProgram=0;
}

Technique::~Technique(){
    for(ShaderObjList::iterator it=shaderObjectList.begin(); it!=shaderObjectList.end(); ++it){
        glDeleteShader(*it);
    }
    if(shaderProgram!=0){
        glDeleteProgram(shaderProgram);
        shaderProgram=0;
    }
}

bool Technique::Init(){
    shaderProgram=glCreateProgram();
    if(shaderProgram==0){
        fprintf(stderr, "Error creating shader program\n");
        return false;
    }
    return true;
}

bool Technique::AddShader(GLenum shaderType, const char* fileName){
    
    std::string shaderText;
    
    if(!ReadFile(fileName,shaderText)){
        fprintf(stderr, "Error reading shader file %s\n", fileName);
        return false;
    }
    
    GLuint ShaderObj = glCreateShader(shaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", shaderType);
        return false;
    }

    const GLchar* p[1];
    p[0] = shaderText.c_str();
    GLint Lengths[1];
    Lengths[0]= shaderText.length();
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d from file %s: '%s'\n", shaderType, fileName, InfoLog);
        return false;
    }

    glAttachShader(shaderProgram, ShaderObj);
    return true;
}


/* After all the shaders have been added link
 * and validate the shader program
 */
bool Technique::Finalize(){
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        return false;
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        return false;
    }

    glUseProgram(shaderProgram);
    return true;
}


void Technique::Enable(){
    glUseProgram(shaderProgram);
}

GLint Technique::GetUniformLocation(const char* uniformName){
    GLuint location=glGetUniformLocation(shaderProgram,uniformName);
    if(location==INVALID_UNIFORM_LOCATION){
        fprintf(stderr,"Warning: Unknown uniform name %s\n",uniformName);
    }
    return location;
}

GLint Technique::GetProgramParam(GLint param){
    GLint ret;
    glGetProgramiv(shaderProgram,param,&ret);
    return ret;
}