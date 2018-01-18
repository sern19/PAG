#version 330 core //Wersja GLSL

layout (location = 0) in vec3 vertexPosition; //Gdzie ma szukać wektora wierzchołków

uniform mat4 MVPMatrix;

out vec3 fragVertexPosition;

void main()
{    
	fragVertexPosition=vertexPosition;
    gl_Position=(MVPMatrix*vec4(vertexPosition, 1.0f)).xyww;
}
