/* 
 * File:   camera.h
 * Author: acordeon
 *
 * Created on April 9, 2014, 12:38 PM
 */

#ifndef CAMERA_H
#define	CAMERA_H
#include "math_3d.h"

const static float StepScale = 0.1f;

class Camera{
public:
    Camera();
    Camera(int WindowWidth, int WindowHeight);
    Camera(int WindowWidth, int WindowHeight, const Vector3f&,const Vector3f&,const Vector3f&);
    const Vector3f& getTarget() const;
    const Vector3f& getUp() const;
    const Vector3f& getPosition() const;
    
    bool onKeyboard(int key );
    void onMouse(int x, int y);
    void onRender();
    
private:
    int windowWidth,windowHeight;
    Vector2i mousePos;
    float angleV, angleH;
    bool onUpperEdge, onLowerEdge, onLeftEdge,onRightEdge;
    Vector3f target;
    Vector3f up;
    Vector3f pos;
    
    void Init();
    void Update();
};

#endif	/* CAMERA_H */

