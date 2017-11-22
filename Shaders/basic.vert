#version 330 core //Wersja GLSL
layout (location = 0) in vec3 vertexPosition; //Gdzie ma szukać wektora wierzchołków
layout (location = 1) in vec3 vertexColor; //Gdzie ma szukać koloru wierzchołków
layout (location = 2) in vec2 vertexTexture; //Gdzie ma szukać współrzędnych tekstur wierzchołków

out vec3 fragVertexColor; //Zmienna wyjściowa koloru wierzchołka
out vec2 fragVertexTexture; //Zmienna wyjściowa współrzędnych tekstury wierzchołka

uniform mat4 wvp;
uniform mat4 model;

void main()
{
    fragVertexColor=vertexColor;
    fragVertexTexture=vertexTexture;
    gl_Position = wvp*model*vec4(vertexPosition, 1.0f); //(vec3,float) lub (float,float,float,float) 1-pozycja w przestrzeni, 0-kierunek w przestrzeni
}
