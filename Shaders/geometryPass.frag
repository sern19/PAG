#version 330 core //Wersja GLSL

in vec3 fragVertexPosition;
in vec3 fragVertexNormal;
in vec3 fragVertexTangent;
in vec3 fragVertexBitangent;
in vec2 fragVertexTexture;

layout (location = 0) out vec3 diffuseOut;
layout (location = 1) out vec3 normalOut;
layout (location = 2) out vec3 positionOut;
layout (location = 3) out vec3 specularColorOut;
layout (location = 4) out vec3 ambientColorOut;
layout (location = 5) out vec3 additionalVarsOut;

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

//Jak narazie silnik nie obsługuje wielu tekstur tego samego typu
uniform sampler2D diffuseTexture[5];
uniform sampler2D specularTexture[5];
uniform sampler2D normalTexture[5];
uniform samplerCube cubeMap;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 ambientColor;
uniform vec3 cameraPos;

uniform int shadingMode;
uniform float shininess;

uniform bool shouldUseDiffuseTexture;
uniform bool shouldUseSpecularTexture;
uniform bool shouldUseNormalTexture;
uniform bool isReflective;

uniform mat3 normalMatrix;

void main()
{
    int i;
    vec4 diffuse;
    vec3 normal, specular;
    
    vec4 texelDiffuse, texelSpecular, texelNormal;
    
    //Generowanie texeli - tu docelowo mieszanie tekstur
    texelDiffuse=texture(diffuseTexture[0], fragVertexTexture);
    texelSpecular=texture(specularTexture[0], fragVertexTexture);
    texelNormal=texture(normalTexture[0], fragVertexTexture);
    texelNormal.rgb=texelNormal.rgb*2.0-1.0; //Konwersja do zakresu [-1,1]

    //Wybór diffuse - tu docelowo mieszanie tekstur
    if (shouldUseDiffuseTexture)
        diffuse=texelDiffuse;
    else
        diffuse=vec4(diffuseColor,1);

    //Wybór specular - tu docelowo mieszanie tekstur
    if (shouldUseSpecularTexture)
        specular=texelSpecular.xyz;
    else
        specular=specularColor;
    
    //Wybór normalnych - tu docelowo mieszanie tekstur
    if (shouldUseNormalTexture)
    {
        mat3 tbn=mat3(normalize(fragVertexTangent), normalize(fragVertexBitangent), normalize(fragVertexNormal));
        normal=normalize(normalMatrix*tbn*texelNormal.xyz);
    }
    else normal=normalize(normalMatrix*fragVertexNormal);
    
    if (!isReflective||shadingMode==SHADING_NONE)
        diffuseOut=diffuse.rgb;
    else
    {
        float air = 1.0;
        float glass = 1.51714;
        vec3 I = normalize(fragVertexPosition - cameraPos);
        vec3 refraction = refract(I, normalize(normal), air/glass);
        vec3 reflection = reflect(I, normalize(normal));
        float r0 = ((air-glass)*(air-glass))/((air+glass)*(air+glass));
        float fresnel=r0 + (1.0 - r0) * pow((1.0 - dot(-I, normal)), 5.0);
        diffuseOut=mix(texture(cubeMap, normalize(refraction)).rgb, texture(cubeMap, normalize(reflection)).rgb, fresnel);
    }
    normalOut=normal;
    positionOut=fragVertexPosition;
    
    if (isReflective)
        specularColorOut=vec3(0,0,0);
    else
        specularColorOut=specular.rgb;
    
    ambientColorOut=ambientColor;
    additionalVarsOut=vec3(0, shadingMode, shininess);
}
