#version 330 core //Wersja GLSL

in vec3 fragVertexColor; //Zmienna pozycji z vertex shadera
in vec2 fragVertexTexture; //Zmienna współrzędnych tekstury z vertex shadera

out vec4 fragColor; //Zmienna wyjściowa dla koloru fragmentu

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main()
{
    vec3 newTexture=texture(myTexture1, fragVertexTexture).rgb*texture(myTexture1, fragVertexTexture).a; //Zastępujemy prezroczystość kolorem czarnym
    fragColor=mix(mix(vec4(newTexture, 1.0f),vec4(fragVertexTexture,1.0f,1.0f),0.5f), mix(texture(myTexture2, fragVertexTexture),vec4(fragVertexTexture,1.0f,1.0f),0.5f),0.5f); //RGBA
}
