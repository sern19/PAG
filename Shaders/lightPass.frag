#version 330 core //Wersja GLSL

//Typy cieniowania
#define SHADING_FLAT 0x1
#define SHADING_GOURAUD 0x2
#define SHADING_PHONG 0x3
#define SHADING_BLINN 0x4
#define SHADING_TOON 0x5
#define SHADING_ORENNAYAR 0x6
#define SHADING_MINNAERT 0x7
#define SHADING_COOKTORRANCE 0x8
#define SHADING_NONE 0x9
#define SHADING_FRESNEL 0xa

int shadingMode=SHADING_BLINN;

uniform vec3 cameraPos;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D positionMap;
uniform sampler2D specularColorMap;
uniform sampler2D ambientColorMap;
uniform sampler2D texCoordsMap;
uniform vec2 screenSize;

out vec4 fragColor;

uniform struct Light
{
    vec4 position; //Pozycja
    vec3 color; //Kolor światła
    float attenuation; //Tłumienie
    float attenuationLin; //Tłumienie liniowe
    float attenuationExp; //Tłumienie exp
    float ambientCoefficient; //"Jasność" ambienta
    float coneAngle; //Kąt spotlighta
    vec3 coneDirection; //Kierunek spotlighta
} light;

vec3 applyPhongLight(const in vec4 inputColor, const vec3 specularColor, const in vec3 ambientColor, const in float shininess, const in vec3 transformedNormal, const in vec3 inputPos, const in vec3 inputToCamera)
{
    vec3 inputToLight, ambient, diffuse, specular;
    float diffuseCoefficient, specularCoefficient; //"Jasności"
    float attenuation; //Tłumienia
    
    //Ambient
    ambient=light.ambientCoefficient*ambientColor*light.color*inputColor.rgb;
    
    if (light.position.w==0)
    {
        //Kierunkowe
        inputToLight=normalize(-light.position.xyz);
        attenuation=1.0;
    }
    else
    {
        ambient=vec3(0,0,0);
        //Punktowe
        float distanceToLight=length(light.position.xyz-inputPos);
        //attenuation=1.0/(1.0+light.attenuation*pow(distanceToLight, 2)); 
        attenuation=1.0f/(1.0f + light.attenuationLin * distanceToLight
                            + light.attenuationExp * distanceToLight * distanceToLight +
                            light.attenuation);
        inputToLight=normalize(light.position.xyz-inputPos);
        
        //Stożkowe
        if (light.coneDirection!=vec3(0))
        {
            float lightToInputAngle=degrees(acos(dot(-inputToLight, normalize(light.coneDirection))));
            if (lightToInputAngle>light.coneAngle)
                attenuation=0.0;
        }
    }
    
    //Diffuse
    diffuseCoefficient=max(dot(transformedNormal, inputToLight), 0.0); //Z przekształceń z cos() -> dot(transformedNormal, inputToLight) / length(transformedNormal)*length(inputToLight) -> To
    diffuse=diffuseCoefficient*light.color*inputColor.rgb;
    
    //Specular
    specularCoefficient=0.0;
    
    if (diffuseCoefficient>0.0) //Gdy ma wartość 0 oznacza, że jest od środka
    {
        if (shadingMode==SHADING_PHONG) specularCoefficient=pow(max(0.0, dot(inputToCamera, reflect(-inputToLight, transformedNormal))), shininess);
        else
        {
            vec3 H=normalize(inputToCamera+inputToLight);
            specularCoefficient=pow(max(0.0,dot(transformedNormal, H)), shininess);
        }
    }
    specular=specularCoefficient*specularColor*light.color;

    return ambient+attenuation*(diffuse+specular);
}

vec4 shadeColor(const in vec4 inputColor, const in vec3 fragVertex, const in vec3 specularColor, const in vec3 ambientColor, const in float shininess, const in vec3 transformedNormal)
{
    vec4 outputColor=vec4(0);
    
    if (shadingMode==SHADING_NONE)
        outputColor=inputColor;
    //else if (shadingMode==SHADING_GOURAUD)
        //outputColor=vec4(1,1,1,1); //Narazie nieobsługiwane
    else if (shadingMode==SHADING_PHONG || shadingMode==SHADING_BLINN)
    {
        int i;
        vec3 inputToCamera=normalize(cameraPos-fragVertex);
        outputColor=vec4(applyPhongLight(inputColor, specularColor, ambientColor, shininess, transformedNormal, fragVertex, inputToCamera),1);
    }
    else
        outputColor=inputColor;
    
    return vec4(outputColor.rgb, inputColor.a);
}

vec2 calcCoords()
{
    return gl_FragCoord.xy/screenSize;
}

void main()
{
    vec2 coords=calcCoords();
    vec4 diffuse=texture(diffuseMap, coords);
    vec3 normal=texture(normalMap, coords).rgb;
    vec3 position=texture(positionMap, coords).rgb;
    float shininess=texture(texCoordsMap, coords).z;
    vec3 specular=texture(specularColorMap, coords).rgb;
    vec3 ambient=texture(ambientColorMap, coords).rgb;
    
    fragColor=shadeColor(diffuse, position, specular, ambient, shininess, normal);
}
