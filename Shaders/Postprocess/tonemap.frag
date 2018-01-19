#version 330 core //Wersja GLSL

uniform sampler2D screenMap;
uniform vec2 screenSize;

out vec4 fragColor;

const float exposure = 2.0f;
const float gamma = 2.2f;

vec2 calcCoords()
{
    return (gl_FragCoord.xy/screenSize);
}

void main()
{
    vec2 coords=calcCoords();
    vec4 outputColor=texture(screenMap, coords);
    
    //Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-outputColor.rgb * exposure);
    //Gamma correction 
    //mapped = pow(mapped, vec3(1.0 / gamma));
    
    fragColor=vec4(mapped.rgb,1);
}
