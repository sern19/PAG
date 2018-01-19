#version 330 core //Wersja GLSL

uniform sampler2D screenMap;
uniform vec2 screenSize;

out vec4 fragColor;

vec2 calcCoords()
{
    return gl_FragCoord.xy/screenSize;
}

void main()
{
    vec2 coords=calcCoords();
    vec4 outputColor=texture(screenMap, coords);
    
    fragColor=vec4(outputColor.bgr,1);
}
