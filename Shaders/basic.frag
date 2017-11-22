#version 330 core //Wersja GLSL

in vec3 fragVertexColor; //Zmienna pozycji z vertex shadera
in vec2 fragVertexTexture; //Zmienna współrzędnych tekstury z vertex shadera

out vec4 fragColor; //Zmienna wyjściowa dla koloru fragmentu

uniform sampler2D diffuse0;
uniform sampler2D diffuse1;

void main()
{
    vec4 texel0, texel1;
    texel0=texture(diffuse0, fragVertexTexture);
    texel1=texture(diffuse1, fragVertexTexture);
    fragColor=texel0;//mix(texel0, texel1, texel1.a);
}
