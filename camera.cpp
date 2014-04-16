/* File Camera.cpp
 * Camera class implementation.
 */

#include <GL/freeglut.h>
#include "camera.h"

Camera::Camera():windowWidth(0),windowHeight(0){
    pos=Vector3f(0.0f,0.0f,0.0f);
    target=Vector3f(0.0f,0.0f,1.0f);
    up=Vector3f(0.0f,1.0f,0.0f);
    Init();
}

Camera::Camera(int WindowWidth, int WindowHeight):
    windowWidth(WindowWidth),windowHeight(WindowHeight)
{
    pos=Vector3f(0.0f,0.0f,0.0f);
    target=Vector3f(0.0f,0.0f,1.0f);
    up=Vector3f(0.0f,1.0f,0.0f);
    Init();
}

Camera::Camera(int WindowWidth, int WindowHeight,
        const Vector3f& positionV, const Vector3f& targetV, const Vector3f& upV):
        windowWidth(WindowWidth),windowHeight(WindowHeight)
{
    pos=positionV;
    target=targetV;
    target.normalize();
    up=upV;
    up.normalize();
    Init();
}

const Vector3f& Camera::getPosition() const{
    return pos;
}

const Vector3f& Camera::getTarget() const{
    return target;
}

const Vector3f& Camera::getUp() const{
    return up;
}

bool Camera::onKeyboard(int key){
    bool result=false;
    switch(key){
        case GLUT_KEY_UP:
            pos+=(target*StepScale);
            result=true;
            break;
        case GLUT_KEY_DOWN:
            pos-=(target*StepScale);
            result=true;
            break;
        case GLUT_KEY_LEFT:
        {
            Vector3f Left = target.cross(up);
            Left.normalize();
            Left *= StepScale;
            pos += Left;
            result = true;
        }
            break;
        case GLUT_KEY_RIGHT:
        {
            Vector3f Right=up.cross(target);
            Right.normalize();
            Right*=StepScale;
            pos+=Right;
            result= true;
        }
            break;
    }
    return result;
    
}

void Camera::Init(){
    Vector3f HTarget(target.x, 0.0, target.z);
    HTarget.normalize();
    
    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
        {
            angleH = 360.0f - ToDegree(asin(HTarget.z));
        }
        else
        {
            angleH = 180.0f + ToDegree(asin(HTarget.z));
        }
    }
    else
    {
        if (HTarget.x >= 0.0f)
        {
            angleH = ToDegree(asin(-HTarget.z));
        }
        else
        {
            angleH = 90.0f + ToDegree(asin(-HTarget.z));
        }
    }
    
    angleV = -ToDegree(asin(target.y));

    onUpperEdge = false;
    onLowerEdge = false;
    onLeftEdge  = false;
    onRightEdge = false;
    mousePos.x  = windowWidth / 2;
    mousePos.y  = windowHeight / 2;

    glutWarpPointer(mousePos.x, mousePos.y);
}