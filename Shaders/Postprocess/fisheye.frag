#version 330 core //Wersja GLSL

uniform sampler2D screenMap;
uniform vec2 screenSize;

out vec4 fragColor;

vec2 calcCoords()
{
    return (gl_FragCoord.xy/screenSize);
}

void main()
{
    vec2 coords=calcCoords();
    vec4 outputColor;
    
    float aperture = 180.0;
    float apertureHalf = 0.5 * aperture * (3.14 / 180.0);
    float maxFactor = sin(apertureHalf);
    
    float zoom=1;
    
    coords/=zoom;
    coords+=(1.0f-(1.0f/zoom))/2.0f;
    
    vec2 uv;
    vec2 xy = 2.0 * coords.xy - 1.0;
    float d = length(xy);
    if (d < (2.0-(maxFactor)))
    {
        d = length(xy * maxFactor);
        float z = sqrt(1.0 - d * d);
        float r = atan(d, z) / 3.14;
        float phi = atan(xy.y, xy.x);
        
        uv.x = r * cos(phi) + 0.5;
        uv.y = r * sin(phi) + 0.5;
        outputColor=texture(screenMap, uv);
    }
    else
    {
        outputColor=vec4(0,0,0,1);
    }
    
    
    fragColor=vec4(outputColor.rgb,1);
}
