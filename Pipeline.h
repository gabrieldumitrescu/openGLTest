/* 
 * File:   Pipeline.h
 * Author: acordeon
 *
 * Created on April 7, 2014, 2:24 PM
 */

#ifndef PIPELINE_H
#define	PIPELINE_H
#include "camera.h"
#include "math_3d.h" 

class Pipeline{
public:
    Pipeline();
    void setScale(float x, float y, float z);
    void setRotation(float x, float y, float z);
    void setTranslation(float x, float y, float z);
    void setPersProj(float FOV, float width, float height,float zNear, float zFar);
    void setCamera(const Vector3f& Pos,const Vector3f& lookAt,const Vector3f& up);
    const Matrix4f* getTransform();
private:
    Vector3f scale,rotation,translation;
    Matrix4f transform;
    PersProjInfo persProj;
    struct{
      Vector3f pos,target,up;  
    }cameraInfo;
    
    
};

#endif	/* PIPELINE_H */

