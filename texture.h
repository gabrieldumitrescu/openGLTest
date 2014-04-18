/* 
 * File:   texture.h
 * Author: quentin
 *
 * Created on April 18, 2014, 7:38 PM
 */

#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>
#include <GL/glew.h>
#include <ImageMagick/Magick++.h>

/**
 * Texture object offers methods to load an image file into
 * an openGL texture to be used in 3D object painting. */
class Texture{
    std::string fileName;
    GLenum textureTarget;
    GLuint textureObject;
    Magick::Image* pImage;
    Magick::Blob blob;
    
public:
    /**
     * Create a texture object from an image file.
     * To use the object you must call load() and bind() 
     * @param texTarget must be GL_TEXTURE_1D, GL_TEXTURE_2D ...
     * @param fName Name of an compatible image file
     */
    Texture(GLenum texTarget, const std::string& fName);
    
    /**
     * Loads the image file into an openGL texture
     * @return true if loading is successful.
     */
    bool load();
    
    /**
     * Bind the texture to a texture unit.
     * @param textureUnit = GL_TEXTURE0, GL_TEXTURE1 ...
     */
    void bind(GLenum textureUnit);
    
};

#endif	/* TEXTURE_H */

