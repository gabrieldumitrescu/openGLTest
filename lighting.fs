#version 330

in vec2 TexCoord0;

out vec4 FragColor;

uniform sampler2D gSampler;

struct DirectionalLight{
    vec3 Color;
    float AmbientIntensity;
};

uniform DirectionalLight gDirectionalLight;

void main()
{
    FragColor = texture2D(gSampler, TexCoord0.xy)*
                vec4(gDirectionalLight.Color,1.0f)*
                gDirectionalLight.AmbientIntensity;
}