#version 330 core //Wersja GLSL

uniform sampler2D screenMap;
uniform sampler2D depthMap;
uniform vec2 screenSize;

out vec4 fragColor;

const float focus=0.001;
const float blurclamp=0.02;  // max blur amount
const float bias=0.05;    //aperture - bigger values for shallower depth of field

vec2 calcCoords()
{
    return (gl_FragCoord.xy/screenSize);
}

void main()
{
    vec2 coords=calcCoords();
    vec2 aspectcorrect=vec2(1.0,screenSize.x/screenSize.y);
    
    float depth=(2.0 * 0.001) / (100.0f + 0.001 - texture(depthMap, coords).x * (100.0f - 0.001)); //Może kiedyś przez uniformy
    
    float factor=(depth-focus);
    vec2 dofblur=vec2(clamp(factor * bias, -blurclamp, blurclamp ));
    
    vec4 outputColor = vec4(0.0);
    
    //Próbkowanie po okręgach
    outputColor += texture(screenMap, coords);
    outputColor += texture(screenMap, coords + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur);
    outputColor += texture(screenMap, coords + (vec2( 0.15,0.37 )*aspectcorrect) * dofblur);
    outputColor += texture(screenMap, coords + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur);
    outputColor += texture(screenMap, coords + (vec2( -0.37,0.15 )*aspectcorrect) * dofblur);    
    outputColor += texture(screenMap, coords + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur);    
    outputColor += texture(screenMap, coords + (vec2( 0.37,-0.15 )*aspectcorrect) * dofblur);    
    outputColor += texture(screenMap, coords + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur);    
    outputColor += texture(screenMap, coords + (vec2( -0.15,-0.37 )*aspectcorrect) * dofblur);
    outputColor += texture(screenMap, coords + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur);    
    outputColor += texture(screenMap, coords + (vec2( -0.15,0.37 )*aspectcorrect) * dofblur);
    outputColor += texture(screenMap, coords + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur);
    outputColor += texture(screenMap, coords + (vec2( 0.37,0.15 )*aspectcorrect) * dofblur);    
    outputColor += texture(screenMap, coords + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur);    
    outputColor += texture(screenMap, coords + (vec2( -0.37,-0.15 )*aspectcorrect) * dofblur);    
    outputColor += texture(screenMap, coords + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur);    
    outputColor += texture(screenMap, coords + (vec2( 0.15,-0.37 )*aspectcorrect) * dofblur);
    
    outputColor += texture(screenMap, coords + (vec2( 0.15,0.37 )*aspectcorrect) * dofblur*0.9);
    outputColor += texture(screenMap, coords + (vec2( -0.37,0.15 )*aspectcorrect) * dofblur*0.9);        
    outputColor += texture(screenMap, coords + (vec2( 0.37,-0.15 )*aspectcorrect) * dofblur*0.9);        
    outputColor += texture(screenMap, coords + (vec2( -0.15,-0.37 )*aspectcorrect) * dofblur*0.9);
    outputColor += texture(screenMap, coords + (vec2( -0.15,0.37 )*aspectcorrect) * dofblur*0.9);
    outputColor += texture(screenMap, coords + (vec2( 0.37,0.15 )*aspectcorrect) * dofblur*0.9);        
    outputColor += texture(screenMap, coords + (vec2( -0.37,-0.15 )*aspectcorrect) * dofblur*0.9);    
    outputColor += texture(screenMap, coords + (vec2( 0.15,-0.37 )*aspectcorrect) * dofblur*0.9);    
    
    outputColor += texture(screenMap, coords + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur*0.7);
    outputColor += texture(screenMap, coords + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur*0.7);    
    outputColor += texture(screenMap, coords + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur*0.7);    
    outputColor += texture(screenMap, coords + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur*0.7);    
    outputColor += texture(screenMap, coords + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur*0.7);
    outputColor += texture(screenMap, coords + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur*0.7);    
    outputColor += texture(screenMap, coords + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur*0.7);    
    outputColor += texture(screenMap, coords + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur*0.7);
    
    outputColor += texture(screenMap, coords + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur*0.4);
    outputColor += texture(screenMap, coords + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur*0.4);    
    outputColor += texture(screenMap, coords + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur*0.4);    
    outputColor += texture(screenMap, coords + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur*0.4);    
    outputColor += texture(screenMap, coords + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur*0.4);
    outputColor += texture(screenMap, coords + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur*0.4);    
    outputColor += texture(screenMap, coords + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur*0.4);    
    outputColor += texture(screenMap, coords + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur*0.4);    
    
    fragColor=vec4(outputColor.rgb/41.0f, 1);
}
