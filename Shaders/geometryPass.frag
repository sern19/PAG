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
layout (location = 5) out vec3 texCoordsOut;

//Jak narazie silnik nie obsługuje wielu tekstur tego samego typu
uniform sampler2D diffuseTexture[5];
uniform sampler2D specularTexture[5];
uniform sampler2D normalTexture[5];

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 ambientColor;

uniform float shininess;

uniform bool shouldUseDiffuseTexture;
uniform bool shouldUseSpecularTexture;
uniform bool shouldUseNormalTexture;

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
    
    diffuseOut=diffuse.rgb;
    normalOut=normal;
    positionOut=fragVertexPosition;
    specularColorOut=specular;
    ambientColorOut=ambientColor;
    texCoordsOut=vec3(fragVertexTexture, shininess);
}
