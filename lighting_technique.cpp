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
    worldMatrixLocation=GetUniformLocation("gWorld");
    samplerLocation=GetUniformLocation("gSampler");
    dirLightLocation.color=GetUniformLocation("gDirectionalLight.Color");
    dirLightLocation.ambientIntensity=GetUniformLocation("gDirectionalLight.AmbientIntensity");
    dirLightLocation.direction=GetUniformLocation("gDirectionalLight.Direction");
    dirLightLocation.diffuseIntensity=GetUniformLocation("gDirectionalLight.DiffuseIntensity");
    eyeWorldPosLocation=GetUniformLocation("gEyeWorldPos");
    matSpecularIntensityLocation=GetUniformLocation("gMatSpecularIntensity");
    matSpecularPowerLocation=GetUniformLocation("gMatSpecularPower");
    
    if(wVPLocation==INVALID_UNIFORM_LOCATION ||
       worldMatrixLocation==INVALID_UNIFORM_LOCATION ||
       samplerLocation==INVALID_UNIFORM_LOCATION ||
       dirLightLocation.color==INVALID_UNIFORM_LOCATION ||
       dirLightLocation.ambientIntensity==INVALID_UNIFORM_LOCATION || 
       dirLightLocation.direction==INVALID_UNIFORM_LOCATION || 
       dirLightLocation.diffuseIntensity==INVALID_UNIFORM_LOCATION ||
       eyeWorldPosLocation==INVALID_UNIFORM_LOCATION ||
       matSpecularIntensityLocation==INVALID_UNIFORM_LOCATION ||
       matSpecularPowerLocation==INVALID_UNIFORM_LOCATION)
        return false;
    return true;
}

void LightingTechnique::setWVP(const Matrix4f& wvp){
    glUniformMatrix4fv(wVPLocation,1,GL_TRUE,(const GLfloat*)wvp.m);
}

void LightingTechnique::setWorldMatrix(const Matrix4f& wvp){
    glUniformMatrix4fv(worldMatrixLocation,1,GL_TRUE,(const GLfloat*)wvp.m);
}

void LightingTechnique::setTextureUnit(unsigned int textureUnit){
    glUniform1i(samplerLocation,textureUnit);
}

void LightingTechnique::setDirectionalLight(const DirectionalLight& dirLight){
    glUniform3f(dirLightLocation.color,dirLight.color.x,dirLight.color.y,dirLight.color.z);
    glUniform1f(dirLightLocation.ambientIntensity , dirLight.ambientIntensity);
    Vector3f direction;
    direction=dirLight.Direction;
    direction.normalize();
    glUniform3f(dirLightLocation.direction,direction.x,direction.y,direction.z);
    glUniform1f(dirLightLocation.diffuseIntensity,dirLight.DiffuseIntensity);
}


void LightingTechnique::setEyeWorldPos(const Vector3f& eyeWorldPos){
    glUniform3f(eyeWorldPosLocation,eyeWorldPos.x,eyeWorldPos.y,eyeWorldPos.z);
}

void LightingTechnique::setMatSpecularIntensity(float intensity){
    glUniform1f(matSpecularIntensityLocation,intensity);
}

void LightingTechnique::setMatSpecularPower(float power){
    glUniform1f(matSpecularPowerLocation,power);
}