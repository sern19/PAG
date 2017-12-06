#version 330 core //Wersja GLSL

layout (location = 0) in vec3 vertexPosition; //Gdzie ma szukać wektora wierzchołków
layout (location = 1) in vec3 vertexNormal; //Gdzie ma szukać normalnych wierzchołków
layout (location = 2) in vec3 vertexTangent; //Gdzie ma szukać normalnych wierzchołków
layout (location = 3) in vec3 vertexBitangent; //Gdzie ma szukać normalnych wierzchołków
layout (location = 4) in vec2 vertexTexture; //Gdzie ma szukać współrzędnych tekstur wierzchołków

smooth out vec3 fragVertex;
smooth out vec3 fragVertexNormal;
smooth out vec3 fragVertexTangent;
smooth out vec3 fragVertexBitangent;
smooth out vec2 fragVertexTexture;

uniform mat4 wvpModelMatrix;
uniform mat4 wvModelMatrix;

void main()
{
    fragVertex=vec3(wvModelMatrix*vec4(vertexPosition, 1.0f)); //Potrzebne potem do operacji na normalnych
    fragVertexNormal=vertexNormal;
    fragVertexTangent=vertexTangent;
    fragVertexBitangent=vertexBitangent;
    fragVertexTexture=vertexTexture;
    
    gl_Position=wvpModelMatrix*vec4(vertexPosition, 1.0f); //(vec3,float) lub (float,float,float,float) 1-pozycja w przestrzeni, 0-kierunek w przestrzeni
}
