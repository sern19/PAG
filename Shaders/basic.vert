#version 330 core //Wersja GLSL

layout (location = 0) in vec3 vertexPosition; //Gdzie ma szukać wektora wierzchołków
layout (location = 1) in vec3 vertexNormal; //Gdzie ma szukać normalnych wierzchołków
layout (location = 2) in vec2 vertexTexture; //Gdzie ma szukać współrzędnych tekstur wierzchołków

out vec3 fragVertex;
out vec3 fragVertexNormal;
out vec2 fragVertexTexture;

uniform mat4 wvpMatrix;
uniform mat4 modelMatrix;

void main()
{
    fragVertex=vertexPosition; //Potrzebne potem do operacji na normalnych
    fragVertexNormal=vertexNormal;
    fragVertexTexture=vertexTexture;
    
    gl_Position=modelMatrix*vec4(vertexPosition, 1.0f); //(vec3,float) lub (float,float,float,float) 1-pozycja w przestrzeni, 0-kierunek w przestrzeni
}
