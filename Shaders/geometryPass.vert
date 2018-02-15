#version 330 core //Wersja GLSL

layout (location = 0) in vec3 vertexPosition; //Gdzie ma szukać wektora wierzchołków
layout (location = 1) in vec3 vertexNormal; //Gdzie ma szukać normalnych wierzchołków
layout (location = 2) in vec3 vertexTangent; //Gdzie ma szukać normalnych wierzchołków
layout (location = 3) in vec3 vertexBitangent; //Gdzie ma szukać normalnych wierzchołków
layout (location = 4) in vec2 vertexTexture; //Gdzie ma szukać współrzędnych tekstur wierzchołków

out vec3 fragVertexPosition;
out vec3 fragVertexNormal;
out vec3 fragVertexTangent;
out vec3 fragVertexBitangent;
out vec2 fragVertexTexture;

uniform mat4 MVPMatrix;
uniform mat4 modelMatrix;

void main()
{
    fragVertexPosition=vec3(modelMatrix*vec4(vertexPosition, 1.0f));
    fragVertexNormal=vertexNormal;
    fragVertexTangent=vertexTangent;
    fragVertexBitangent=vertexBitangent;
    fragVertexTexture=vertexTexture;
    
    gl_Position=MVPMatrix*vec4(vertexPosition, 1.0f); //(vec3,float) lub (float,float,float,float) 1-pozycja w przestrzeni, 0-kierunek w przestrzeni
}
