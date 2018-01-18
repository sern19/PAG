#version 330 core //Wersja GLSL

uniform samplerCube cubeMap;

in vec3 fragVertexPosition;

out vec4 fragColor;

void main()
{
    fragColor=texture(cubeMap, fragVertexPosition);
}
