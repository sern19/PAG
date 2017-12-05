#version 330 core //Wersja GLSL

#define MAXIMUM_NUMBER_OF_LIGHTS 16

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

in vec3 fragVertex;
in vec2 fragVertexTexture;
in vec3 fragVertexNormal;

//Jak narazie silnik nie obsługuje wielu tekstur tego samego typu
uniform sampler2D diffuse[10];
uniform sampler2D specular[10];
uniform sampler2D normal[10];

uniform int shadingMode;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 ambientColor;

uniform float shininess;
uniform float shininessStrength;

uniform bool shouldUseDiffuseTexture;
uniform bool shouldUseNormalTexture;
uniform bool shouldUseSpecularTexture;

uniform mat3 normalMatrix;

uniform mat4 modelMatrix;

out vec4 fragColor;

uniform struct Light
{
    vec4 position;
    vec3 color;
} lights[MAXIMUM_NUMBER_OF_LIGHTS];

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
    texel0=texture(diffuse[0], fragVertexTexture);
    if (shouldUseDiffuseTexture)
        fragColor=shadeColor(texel0);
    else
        fragColor=shadeColor(vec4(diffuseColor,1));
}
