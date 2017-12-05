#version 330 core //Wersja GLSL

#define MAXIMUM_NUMBER_OF_LIGHTS 16

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

in vec3 fragVertex;
in vec2 fragVertexTexture;
in vec3 fragVertexNormal;

uniform vec3 cameraPosition;

//Jak narazie silnik nie obsługuje wielu tekstur tego samego typu
uniform sampler2D diffuseTexture[10];
uniform sampler2D specularTexture[10];
uniform sampler2D normalTexture[10];

uniform int shadingMode;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 ambientColor;

uniform float shininess;
uniform float shininessStrength;

uniform bool shouldUseDiffuseTexture;
uniform bool shouldUseSpecularTexture;
uniform bool shouldUseNormalTexture;

uniform mat3 normalMatrix;

uniform mat4 modelMatrix;

out vec4 fragColor;

uniform int numberOfActiveLights;
uniform struct Light
{
    vec4 position; //Pozycja
    vec3 color; //Kolor światła
    float attenuation; //Tłumienie
    float ambientCoefficient; //"Jasność" ambienta
} lights[MAXIMUM_NUMBER_OF_LIGHTS];

vec3 applyPhongLight(Light light, const in vec4 inputColor, const vec3 specularColor, const in vec3 transformedNormal, const vec3 inputPos, const vec3 inputToCamera)
{
    vec3 inputToLight, ambient, diffuse, specular;
    float diffuseCoefficient, specularCoefficient; //"Jasności"
    
    //Punktowe
    float distanceToLight=length(light.position.xyz-inputPos);
    float attenuation=1.0/(1.0+light.attenuation*pow(distanceToLight,2)); //Z funkcji tłumienia światła, dodajemy 1 gdy odległość jest równa 0, dodatkowo mnożymy ją przez procentowe tłumienie światła (0.0 - 1.0)
    inputToLight=normalize(light.position.xyz-inputPos);
    
    //Ambient
    ambient=light.ambientCoefficient*ambientColor*inputColor.rgb*light.color;
    
    //Diffuse
    diffuseCoefficient=max(0.0, dot(transformedNormal, inputToLight)); //Z przekształceń z cos() -> dot(transformedNormal, inputToLight) / length(transformedNormal)*length(inputToLight) -> To
    diffuse=diffuseCoefficient*inputColor.rgb*light.color;
    
    //Specular
    specularCoefficient=0.0;
    if (diffuseCoefficient>0.0) //Gdy ma wartość 0 oznacza, że jest od środka
        specularCoefficient=pow(max(0.0, dot (inputToCamera, reflect(-inputToLight, transformedNormal))), shininess*shininessStrength);
    specular=specularCoefficient*specularColor*light.color;
    
    return (ambient+attenuation*(diffuse+specular));
}

vec4 shadeColor(const in vec4 inputColor, const vec3 specularColor, const in vec3 transformedNormal)
{
    vec4 outputColor=vec4(0);
    
    if (shadingMode==SHADING_NONE)
        outputColor=inputColor;
    else if (shadingMode==SHADING_GOURAUD)
        outputColor=vec4(1,1,1,1); //Narazie nieobsługiwane
    else if (shadingMode==SHADING_PHONG)
    {
        int i;
        vec3 inputPos=vec3(modelMatrix*vec4(fragVertex, 1.0f));
        vec3 inputToCamera=normalize(cameraPosition-inputPos);
        for (i=0;i<numberOfActiveLights;i++)
            outputColor+=vec4(applyPhongLight(lights[i], inputColor, specularColor, transformedNormal, inputPos, inputToCamera),0);
    }
    
    return vec4(outputColor.rgb, inputColor.a);
}

void main()
{
    int i;
    vec4 diffuse;
    vec3 specular, normal;
    
    vec4 texelDiffuse, texelSpecular, texelNormal;
    
    //Generowanie texeli - tu docelowo mieszanie tekstur
    texelDiffuse=texture(diffuseTexture[0], fragVertexTexture);
    texelSpecular=texture(specularTexture[0], fragVertexTexture);
    texelNormal=texture(normalTexture[0], fragVertexTexture);

    //Wybór diffuse - tu docelowo mieszanie tekstur
    if (shouldUseDiffuseTexture)
        diffuse=texelDiffuse;
    else
        diffuse=vec4(diffuseColor,1);

    //Wybór specular - tu docelowo mieszanie tekstur
    if (shouldUseSpecularTexture)
        specular=texelSpecular.xyz;
    else
        specular=specularColor;

    //Wybór normalnych - tu docelowo mieszanie tekstur
    if (shouldUseNormalTexture)
        normal=normalize(normalMatrix*texelNormal.xyz);
    else
        normal=normalize(normalMatrix*fragVertexNormal);
    
    fragColor=shadeColor(diffuse, specular, normal);
}
