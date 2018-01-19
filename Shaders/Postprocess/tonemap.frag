#version 330 core //Wersja GLSL

uniform sampler2D screenMap;
uniform vec2 screenSize;

uniform float renderScale;

out vec4 fragColor;

vec2 calcCoords()
{
    return (gl_FragCoord.xy/renderScale)/screenSize;
}

void main()
{
    vec2 coords=calcCoords();
    vec4 outputColor;
    
    fragColor=vec4(outputColor.rgb,1);
}
