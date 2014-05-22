/*File lighting_technique.cpp
 * LightingTechnique class implementation.
 */


#include "lighting_technique.h"

LightingTechnique::LightingTechnique(){
}

bool LightingTechnique::Init(){
    if(!Technique::Init() || 
       !AddShader(GL_VERTEX_SHADER,"lighting.vs") ||
       !AddShader(GL_FRAGMENT_SHADER,"lighting.fs") ||
       !Technique::Finalize())
        return false;
    wVPLocation=GetUniformLocation("gWVP");
    samplerLocation=GetUniformLocation("gSampler");
    dirLightLocation.color=GetUniformLocation("gDirectionalLight.Color");
    dirLightLocation.ambientIntensity=GetUniformLocation("gDirectionalLight.AmbientIntensity");
    dirLightLocation.direction=GetUniformLocation("gDirectionalLight.Direction");
    dirLightLocation.diffuseIntensity=GetUniformLocation("gDirectionalLight.DiffuseIntensity");
    
    if(wVPLocation==INVALID_UNIFORM_LOCATION ||
       samplerLocation==INVALID_UNIFORM_LOCATION ||
       dirLightLocation.color==INVALID_UNIFORM_LOCATION ||
       dirLightLocation.ambientIntensity==INVALID_UNIFORM_LOCATION || 
       dirLightLocation.direction==INVALID_UNIFORM_LOCATION || 
       dirLightLocation.diffuseIntensity==INVALID_UNIFORM_LOCATION)
        return false;
    return true;
}

void LightingTechnique::setWVP(const Matrix4f& wvp){
    glUniformMatrix4fv(wVPLocation,1,GL_TRUE,(const GLfloat*)wvp.m);
}

void LightingTechnique::setTextureUnit(unsigned int textureUnit){
    glUniform1i(samplerLocation,textureUnit);
}

void LightingTechnique::setDirectionalLight(const DirectionalLight& dirLight){
    glUniform3f(dirLightLocation.color,dirLight.color.x,dirLight.color.y,dirLight.color.z);
    glUniform1f(dirLightLocation.ambientIntensity , dirLight.ambientIntensity);
    Vector3f direction;
    direction=dirLight.Direction;
    glUniform3f(dirLightLocation.direction,direction.x,direction.y,direction.z);
    glUniform1f(dirLightLocation.diffuseIntensity,dirLight.DiffuseIntensity);
}
