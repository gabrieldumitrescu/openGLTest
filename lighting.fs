#version 330

in vec2 TexCoord0;

in vec3 Normal0;

out vec4 FragColor;

uniform sampler2D gSampler;

struct DirectionalLight{
    vec3 Color;
    float AmbientIntensity;
    vec3 Direction;
    float DiffuseIntensity;
};

uniform DirectionalLight gDirectionalLight;

void main()
{
    vec4 AmbientColor=vec4(gDirectionalLight.Color,1.0f)*
                gDirectionalLight.AmbientIntensity;
    float DiffuseFactor=dot(normalize(Normal0),-gDirectionalLight.Direction);
    vec4 DiffuseColor;
    if(DiffuseFactor>0){
        DiffuseColor=vec4(gDirectionalLight.Direction,1.0) * 
                    gDirectionaLight.DiffuseIntensity * DiffuseFactor;
    }
    else{
        DiffuseColor=vec4(0.0,0.0,0.0,0.0);
    }
    FragColor = texture2D(gSampler, TexCoord0.xy)*(AmbientColor + DiffuseColor);
}