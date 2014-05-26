/* 
 * File:   lighting_technique.h
 * Author: acordeon
 *
 * Created on April 26, 2014, 12:30 PM
 */

#ifndef LIGHTING_TECHNIQUE_H
#define	LIGHTING_TECHNIQUE_H

#include "math_3d.h"
#include "technique.h"

struct DirectionalLight{
    Vector3f color;
    float ambientIntensity;
    Vector3f Direction;
    float DiffuseIntensity;
};

class LightingTechnique: public Technique{
public:
    LightingTechnique();
    virtual bool Init();
    void setWVP(const Matrix4f& wvp);
    void setWorldMatrix(const Matrix4f& wvp);
    void setTextureUnit(unsigned int textureUnit);
    void setDirectionalLight(const DirectionalLight& dirLight);
    void setEyeWorldPos(const Vector3f& eyeWorldPos);
    void setMatSpecularIntensity(float intensity);
    void setMatSpecularPower(float power);
private:
    GLuint wVPLocation,
           worldMatrixLocation,
           samplerLocation,
           eyeWorldPosLocation,
           matSpecularIntensityLocation,
           matSpecularPowerLocation;
           
    struct {
        GLuint color,ambientIntensity;
        GLuint direction,diffuseIntensity;
        
    }dirLightLocation;
    
};


#endif	/* LIGHTING_TECHNIQUE_H */

