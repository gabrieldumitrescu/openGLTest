/* File Pipeline.cpp
 * Pipeline class implementation
 */

#include "Pipeline.h"
#include "camera.h"



Pipeline::Pipeline(){
    scale=Vector3f(1.0f,1.0f,1.0f);
    rotation=Vector3f();
    translation=Vector3f();
}

void Pipeline::setScale(float x, float y, float z){
    scale.x=x;
    scale.y=y;
    scale.z=z;
}

void Pipeline::setRotation(float x, float y, float z){
    rotation.x=x;
    rotation.y=y;
    rotation.z=z;
}

void Pipeline::setTranslation(float x, float y, float z){
    translation.x=x;
    translation.y=y;
    translation.z=z;
}

void Pipeline::setPersProj(float FOV, float width, float height, float zNear, float zFar){
    persProj.FOV=FOV;
    persProj.width=width;
    persProj.height=height;
    persProj.zNear=zNear;
    persProj.zFar=zFar;
}

void Pipeline::setCamera(const Vector3f& pos,const Vector3f& target,const Vector3f& up){
    cameraInfo.pos=pos;
    cameraInfo.target=target;
    cameraInfo.up=up;
}

const Matrix4f* Pipeline::getTransform(){
    Matrix4f scMat,rotMat,transMat,persProjMat,cameraTranslMat,cameraTransfMat;
    scMat.initScale(scale.x,scale.y,scale.z);
    rotMat.initRotation(rotation.x,rotation.y,rotation.z);
    transMat.initTranslation(translation.x,translation.y,translation.z);
    persProjMat.initPerspProj(persProj);
    cameraTranslMat.initTranslation(-cameraInfo.pos.x,-cameraInfo.pos.y,-cameraInfo.pos.z);
    cameraTransfMat.initCameraTransf(cameraInfo.target,cameraInfo.up);
    transform=persProjMat*cameraTransfMat*cameraTranslMat*transMat*rotMat*scMat;
    return &transform;
}
