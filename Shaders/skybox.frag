#version 330 core //Wersja GLSL

uniform samplerCube cubeMap;

uniform float intensity;

in vec3 fragVertexPosition;

out vec4 fragColor;

void main()
{
    vec3 skyboxTexel=texture(cubeMap, fragVertexPosition).rgb;
    fragColor=vec4(skyboxTexel*intensity,1.0f);
}
