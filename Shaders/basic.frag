#version 330 core //Wersja GLSL

in vec2 fragVertexTexture; //Zmienna współrzędnych tekstury z vertex shadera

out vec4 fragColor; //Zmienna wyjściowa dla koloru fragmentu

uniform sampler2D diffuse0;

uniform bool shouldUseDiffuseTexture;
uniform vec3 diffuseColor;

void main()
{
    vec4 texel0;
    texel0=texture(diffuse0, fragVertexTexture);
    if (shouldUseDiffuseTexture)
        fragColor=texel0;
    else
        fragColor=vec4(diffuseColor,1);
}
