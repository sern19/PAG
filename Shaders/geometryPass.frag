#version 330 core //Wersja GLSL

in vec3 fragVertexPosition;
in vec3 fragVertexNormal;
in vec3 fragVertexTangent;
in vec3 fragVertexBitangent;
in vec2 fragVertexTexture;

layout (location = 0) out vec4 diffuseOut;
layout (location = 1) out vec4 normalOut;
layout (location = 2) out vec4 positionOut;
layout (location = 3) out vec4 texCoordOut;

//Jak narazie silnik nie obsługuje wielu tekstur tego samego typu
uniform sampler2D diffuseTexture[5];
uniform sampler2D specularTexture[5];
uniform sampler2D normalTexture[5];

uniform vec3 diffuseColor;

uniform float shininess;

uniform bool shouldUseDiffuseTexture;
uniform bool shouldUseNormalTexture;

uniform mat3 normalMatrix;

void main()
{
    int i;
    vec4 diffuse;
    vec4 normal;
    
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

    //Wybór normalnych - tu docelowo mieszanie tekstur
    if (shouldUseNormalTexture)
    {
        mat3 tbn=mat3(normalize(fragVertexTangent), normalize(fragVertexBitangent), normalize(fragVertexNormal));
        normal=vec4(normalize(normalMatrix*tbn*texelNormal.xyz), shininess);
    }
    else
        normal=vec4(normalize(normalMatrix*fragVertexNormal), shininess);
    
    diffuseOut=diffuse;
    normalOut=vec4(normal.rgb, 1);
    positionOut=vec4(fragVertexPosition,1);
    texCoordOut=vec4(fragVertexTexture, shininess,1);
}
