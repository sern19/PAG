#version 330 core //Wersja GLSL

//Typy cieniowania
#define SHADING_FLAT 0x1
#define SHADING_GOURAUD 0x2
#define SHADING_PHONG 0x3
#define SHADING_BLINN 0x4
#define SHADING_TOON 0x5
#define SHADING_ORENNAYAR 0x6
#define SHADING_MINNAERT 0x7
#define SHADING_COOKTORRANCE 0x8
#define SHADING_NONE 0x9
#define SHADING_FRESNEL 0xa

in vec2 fragVertexTexture; //Zmienna współrzędnych tekstury z vertex shadera

out vec4 fragColor; //Zmienna wyjściowa dla koloru fragmentu

uniform sampler2D diffuse0;

uniform int shadingMode;

uniform bool shouldUseDiffuseTexture;
uniform vec3 diffuseColor;

vec4 shadeColor(const in vec4 inputColor)
{
    vec4 outputColor;
    
    if (shadingMode==SHADING_NONE)
        outputColor=inputColor;
    else //Tylko dla testowania
        outputColor=vec4(1,1,1,1);
    
    return outputColor;
}

void main()
{
    vec4 texel0;
    texel0=texture(diffuse0, fragVertexTexture);
    if (shouldUseDiffuseTexture)
        fragColor=shadeColor(texel0);
    else
        fragColor=shadeColor(vec4(diffuseColor,1));
}
