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

void Camera::Update(){
    const Vector3f Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f View(1.0f, 0.0f, 0.0f);
    View.rotate(angleH, Vaxis);
    View.normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f Haxis = Vaxis.cross(View);
    Haxis.normalize();
    View.rotate(angleV, Haxis);
       
    target = View;
    target.normalize();

    up = target.cross(Haxis);
    up.normalize();
}

void Camera::onRender(){
     bool ShouldUpdate = false;

    if (onLeftEdge) {
        angleH -= 0.1f;
        ShouldUpdate = true;
    }
    else if (onRightEdge) {
        angleH += 0.1f;
        ShouldUpdate = true;
    }

    if (onUpperEdge) {
        if (angleV > -90.0f) {
            angleV -= 0.1f;
            ShouldUpdate = true;
        }
    }
    else if (onLowerEdge) {
        if (angleV < 90.0f) {
           angleV += 0.1f;
           ShouldUpdate = true;
        }
    }

    if (ShouldUpdate) {
        Update();
    }
}

void Camera::onMouse(int x, int y){
    const int DeltaX = x - mousePos.x;
    const int DeltaY = y - mousePos.y;

    mousePos.x = x;
    mousePos.y = y;

    angleH += (float)DeltaX / 1000.0f;
    angleV += (float)DeltaY / 1000.0f;

    if (DeltaX == 0) {
        if (x <= MARGIN) {
        //    m_AngleH -= 1.0f;
            onLeftEdge = true;
        }
        else if (x >= (windowWidth - MARGIN)) {
        //    m_AngleH += 1.0f;
            onRightEdge = true;
        }
    }
    else {
        onLeftEdge = false;
        onRightEdge = false;
    }

    if (DeltaY == 0) {
        if (y <= MARGIN) {
            onUpperEdge = true;
        }
        else if (y >= (windowHeight - MARGIN)) {
            onLowerEdge = true;
        }
    }
    else {
        onUpperEdge = false;
        onLowerEdge = false;
    }

    Update();
}