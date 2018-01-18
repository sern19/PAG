#version 330 core //Wersja GLSL

layout (location = 0) in vec3 vertexPosition; //Gdzie ma szukać wektora wierzchołków
layout (location = 1) in vec3 vertexNormal; //Gdzie ma szukać normalnych wierzchołków
layout (location = 2) in vec3 vertexTangent; //Gdzie ma szukać normalnych wierzchołków
layout (location = 3) in vec3 vertexBitangent; //Gdzie ma szukać normalnych wierzchołków
layout (location = 4) in vec2 vertexTexture; //Gdzie ma szukać współrzędnych tekstur wierzchołków

out Data
{
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    mat4 mvp;
} vdata;

uniform mat4 MVPMatrix;

void main()
{
    vdata.mvp=MVPMatrix;
    vdata.position=vertexPosition;
    vdata.normal=normalize(vertexNormal);
    vdata.tangent=normalize(vertexTangent);
    vdata.bitangent=normalize(vertexBitangent);
}
