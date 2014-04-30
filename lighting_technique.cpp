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
    dirLightColorLocation=GetUniformLocation("gDirectionalLight.Color");
    dirLightAmbientIntensityLocation=GetUniformLocation("gDirectionalLight.AmbientIntensity");
    if(wVPLocation==INVALID_UNIFORM_LOCATION ||
       samplerLocation==INVALID_UNIFORM_LOCATION ||
       dirLightColorLocation==INVALID_UNIFORM_LOCATION ||
       dirLightAmbientIntensityLocation==INVALID_UNIFORM_LOCATION)
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
    glUniform3f(dirLightColorLocation,dirLight.color.x,dirLight.color.y,dirLight.color.z);
    glUniform1f(dirLightAmbientIntensityLocation , dirLight.ambientIntensity);
}
