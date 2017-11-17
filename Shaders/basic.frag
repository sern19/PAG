#version 330 core //Wersja GLSL

in vec3 fragVertexColor; //Zmienna pozycji z vertex shadera
in vec2 fragVertexTexture; //Zmienna współrzędnych tekstury z vertex shadera

out vec4 fragColor; //Zmienna wyjściowa dla koloru fragmentu

uniform sampler2D myTexture;

void main()
{
    fragColor=mix(texture(myTexture, fragVertexTexture), vec4(fragVertexColor, 1.0f), 0.5f); //RGBA
}
