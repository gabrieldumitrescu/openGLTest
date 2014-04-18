/* File texture.cpp
 * Texture class implementation.
 */

#include <stdio.h>
#include "texture.h"

Texture::Texture(GLenum texTarget, const std::string& fName){
    fileName=fName;
    textureTarget=texTarget;
    pImage=NULL;
}

bool Texture::load(){
    try{
        pImage=new Magick::Image(fileName);
        pImage->write(&blob,"RGBA");
    }
    catch(Magick::Error& error){
        fprintf(stderr, "Error loading texture from file:%s \n %s\n",fileName.c_str(),error.what());
        return false;
    }
    glGenTextures(1,&textureObject);
    glBindTexture(textureTarget, textureObject);
    glTexImage2D(textureTarget,0,GL_RGBA,pImage->columns(),pImage->rows(),0, GL_RGBA,GL_UNSIGNED_BYTE,blob.data());
    glTexParameterf(textureTarget,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(textureTarget,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    return true;
}

void Texture::bind(GLenum textureUnit){
    glActiveTexture(textureUnit);
    glBindTexture(textureTarget,textureObject);
}