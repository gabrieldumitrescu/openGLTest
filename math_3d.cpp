/* File math_3d.cpp
 * Vertex & Matrix functions
 */


#include "math_3d.h"

Vector3f& Vector3f::operator+=(const Vector3f& right){
    x+=right.x;
    y+=right.y;
    z+=right.z;
    return *this;
}

Vector3f& Vector3f::operator-=(const Vector3f& right){
    x-=right.x;
    y-=right.y;
    z-=right.z;
    return *this;
}

Vector3f& Vector3f::operator*=(float scalar){
    x*=scalar;
    y*=scalar;
    z*=scalar;
    return *this;
}

void Vector3f::normalize(){
    const float mod=sqrtf(x*x + y*y + z*z);
    x/=mod;
    y/=mod;
    z/=mod;
}

Vector3f Vector3f::cross(const Vector3f& right) const {
    const float _x = y * right.z - z * right.y;
    const float _y = z * right.x - x * right.z;
    const float _z = x * right.y - y * right.x;

    return Vector3f(_x, _y, _z);
    
}

void Vector3f::rotate(float angle, const Vector3f& axis){
    const float sinHalfAngle=sinf(ToRadian(angle/2));
    const float cosHalfAngle=cosf(ToRadian(angle/2));
    
    const float rx=axis.x*sinHalfAngle;
    const float ry=axis.y*sinHalfAngle;
    const float rz=axis.z*sinHalfAngle;
    const float rw=cosHalfAngle;
    Quaternion rotation(rx,ry,rz,rw);
    Quaternion rotConjugate=rotation.conjugate();
    
    Quaternion result=rotation * (*this) * rotConjugate;
    x=result.x;
    y=result.y;
    z=result.z;
}



void Matrix4f::initIdentity(){
        m[0][0]=1.0f;m[0][1]=0.0f;m[0][2]=0.0f;m[0][3]=0.0f;
        m[1][0]=0.0f;m[1][1]=1.0f;m[1][2]=0.0f;m[1][3]=0.0f;
        m[2][0]=0.0f;m[2][1]=0.0f;m[2][2]=1.0f;m[2][3]=0.0f;
        m[3][0]=0.0f;m[3][1]=0.0f;m[3][2]=0.0f;m[3][3]=1.0f;
}
void Matrix4f::initScale(float x, float y, float z){
        initIdentity();
        m[0][0]=x;
        m[1][1]=y;
        m[2][2]=z;
}
    
void Matrix4f::initTranslation(float x, float y, float z){
    initIdentity();
    m[0][3]=x;
    m[1][3]=y;
    m[2][3]=z;
}


void Matrix4f::initRotationX(float radians){
    initIdentity();
    m[1][1]=cosf(radians);
    m[2][1]=sinf(radians);
    m[1][2]=-sinf(radians);
    m[2][2]=cosf(radians);
}

void Matrix4f::initRotationY(float radians){
    initIdentity();
    m[0][0]=cosf(radians);
    m[2][0]=sinf(radians);
    m[0][2]=-sinf(radians);
    m[2][2]=cosf(radians);
}

void Matrix4f::initRotationZ(float radians){
    initIdentity();
    m[0][0]=cosf(radians);
    m[1][0]=sinf(radians);
    m[0][1]=-sinf(radians);
    m[1][1]=cosf(radians);
}

void Matrix4f::initRotation(float x, float y, float z){
    Matrix4f rx,ry,rz;
    rx.initRotationX(ToRadian(x));
    ry.initRotationY(ToRadian(y));
    rz.initRotationZ(ToRadian(z));
    *this=rz*ry*rx;
}

void Matrix4f::initPerspProj(const PersProjInfo& info){
    const float ar=info.width/info.height;
    const float zRange=info.zNear-info.zFar;
    const float tanHalfFOV=tanf(ToRadian(info.FOV/2.0f));
    initIdentity();
    m[0][0]=1.0f/(tanHalfFOV*ar);
    m[1][1]=1.0f/tanHalfFOV;
    m[2][2]=(-info.zNear-info.zFar)/zRange;
    m[2][3]=2.0f*info.zNear*info.zFar/zRange;
    m[3][2]=1.0f;
    m[3][3]=0.0f;     
}

void Matrix4f::initCameraTransf(const Vector3f& lookAt,const Vector3f& up){
    Vector3f N=lookAt;
    N.normalize();
    Vector3f U=up;
    U.normalize();
    U=U.cross(lookAt);
    Vector3f V=N.cross(U);
    m[0][0] = U.x; m[0][1] = U.y; m[0][2] = U.z; m[0][3] = 0.0f;
    m[1][0] = V.x; m[1][1] = V.y; m[1][2] = V.z; m[1][3] = 0.0f;
    m[2][0] = N.x; m[2][1] = N.y; m[2][2] = N.z; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;    
}

Quaternion::Quaternion(float X, float Y, float Z, float W):x(X),y(Y),z(Z),w(W){}

void Quaternion::normalize(){
    const float len=sqrtf(x*x+y*y+z*z+w*w);
    x/=len;
    y/=len;
    z/=len;
    w/=len;
}

Quaternion Quaternion::conjugate(){
    Quaternion ret(-x,-y,-z,w);
    return ret;
}

Quaternion operator*(const Quaternion& left, const Quaternion& right){
    const float x=left.x*right.w+left.w*right.x+left.y*right.z-left.z*right.y;
    const float y=left.y*right.w+left.w*right.y+left.z*right.x-left.x*right.z;
    const float z=left.z*right.w+left.w*right.z+left.x*right.y-left.y*right.x;
    const float w=left.w*right.w-left.x*right.x-left.y*right.y-left.z*right.z;
    return Quaternion(x,y,z,w);
}

Quaternion operator*(const Quaternion& q, const Vector3f& v){
    const float x=  (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
    const float y=  (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
    const float z=  (q.w * v.z) + (q.x * v.y) - (q.y * v.x);
    const float w=- (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
    return Quaternion(x,y,z,w);
    
}