/* 
 * File:   math_3d.h
 * Author: acordeon
 *
 * Created on April 7, 2014, 9:51 AM
 */

#ifndef MATH_3D_H
#define	MATH_3D_H

#include <math.h>

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI) 


struct Vector2i{
    int x,y;
};

struct PersProjInfo{
    float FOV, width,height,zNear,zFar;
};



struct Vector3f{
    float x,y,z;
    Vector3f():x(0.0f),y(0.0f),z(0.0f){}
    Vector3f(float X, float Y, float Z):x(X),y(Y),z(Z){}
    
    Vector3f& operator+=(const Vector3f& right);
    Vector3f& operator-=(const Vector3f& right);
    Vector3f& operator*=(float scalar);

    void normalize();
    Vector3f cross(const Vector3f& right) const ;
    void rotate(float angle,const Vector3f& axis);
};


inline Vector3f operator*(const Vector3f& v, float scalar){
    Vector3f result;
    result.x=v.x*scalar;
    result.y=v.y*scalar;
    result.z=v.z*scalar;
    return result;
}

struct Matrix4f{
    float m[4][4];
    void initIdentity();
    void initScale(float x, float y, float z);    
    void initTranslation(float x, float y, float z);
    void initRotationX(float radians);    
    void initRotationY(float radians);    
    void initRotationZ(float radians);    
    void initRotation(float x, float y, float z);    
    void initPerspProj(const PersProjInfo& info);
    void initCameraTransf(const Vector3f& lookAt,const Vector3f& up);
    
    inline Matrix4f operator*(const Matrix4f& Right) const{
        Matrix4f Ret;
        for (unsigned int i = 0 ; i < 4 ; i++) {
            for (unsigned int j = 0 ; j < 4 ; j++) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                          m[i][1] * Right.m[1][j] +
                          m[i][2] * Right.m[2][j] +
                          m[i][3] * Right.m[3][j];
            }
        }

        return Ret; 
    }
};

struct Quaternion{
    float x,y,z,w;
    Quaternion(float X, float Y, float Z, float W);
    void normalize();
    Quaternion conjugate();
};

Quaternion operator*(const Quaternion& left, const Quaternion& right);

Quaternion operator*(const Quaternion& q, const Vector3f& v);

#endif	/* MATH_3D_H */

