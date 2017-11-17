#version 330 core //Wersja GLSL

in vec3 fragVertexColor; //Zmienna pozycji z vertex shadera
in vec2 fragVertexTexture; //Zmienna współrzędnych tekstury z vertex shadera

out vec4 fragColor; //Zmienna wyjściowa dla koloru fragmentu

uniform sampler2D myTexture;

void main()
{
    //Aby mieszanie tekstury z przezroczystościa oraz koloru vertexów zaszło poprawnie stosujemy mały trick
    vec3 newTexture=texture(myTexture, fragVertexTexture).rgb*texture(myTexture, fragVertexTexture).a; //Zastępujemy prezroczystość kolorem czarnym
    fragColor=mix(vec4(newTexture, 1.0f), vec4(fragVertexColor, 1.0f), 0.5f); //RGBA
}
