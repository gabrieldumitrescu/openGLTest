#version 330

in vec2 TexCoord0;

in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

uniform sampler2D gSampler;

struct DirectionalLight{
    vec3 Color;
    float AmbientIntensity;
    vec3 Direction;
    float DiffuseIntensity;
};

uniform DirectionalLight gDirectionalLight;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gMatSpecularPower;

void main()
{
    vec4 AmbientColor=vec4(gDirectionalLight.Color,1.0f)*
                gDirectionalLight.AmbientIntensity;

    vec3 LightDirection=-gDirectionalLight.Direction;
    vec3 Normal=normalize(Normal0);

    float DiffuseFactor=dot(Normal,LightDirection);

    vec4 DiffuseColor=vec4(0.0,0.0,0.0,0.0);
    vec4 SpecularColor=vec4(0.0,0.0,0.0,0.0);

    if(DiffuseFactor>0){
        DiffuseColor=vec4(gDirectionalLight.Color,1.0) * 
                    gDirectionalLight.DiffuseIntensity * DiffuseFactor;
        vec3 VertexToEye=normalize(gEyeWorldPos-WorldPos0);
        vec3 LightReflect=normalize(reflect(gDirectionalLight.Direction, Normal));
        float SpecularFactor=dot(VertexToEye, LightReflect);
        SpecularFactor=pow(SpecularFactor,gMatSpecularPower);
        if(SpecularFactor > 0){
            SpecularColor=vec4(gDirectionalLight.Color,1.0) * gMatSpecularIntensity *
                          SpecularFactor;
        }

    }
    
    FragColor = texture2D(gSampler, TexCoord0.xy)*(AmbientColor + DiffuseColor+SpecularColor);
}