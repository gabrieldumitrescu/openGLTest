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
    FragColor = texture2D(gSampler, TexCoord0.xy)*
                vec4(gDirectionalLight.Color,1.0f)*
                gDirectionalLight.AmbientIntensity;
}