#version 330

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 LocalPos0;
in vec3 Tangent0;  

out vec4 FragColor;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
    vec3 Direction;
};

struct Material
{
    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
};

uniform DirectionalLight gDirectionalLight;
uniform Material gMaterial;
uniform sampler2D gSampler;
uniform sampler2D gSamplerSpecularExponent;
uniform sampler2D gNormalMap;  
uniform vec3 gCameraLocalPos;

vec3 CalcBumpedNormal()                                                                     
{                                                                                           
    vec3 Normal = normalize(Normal0);                                                       
    vec3 Tangent = normalize(Tangent0);                                                     
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);                           
    vec3 Bitangent = cross(Tangent, Normal);                                                
    vec3 BumpMapNormal = texture(gNormalMap, TexCoord0).xyz;                                
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);                              
    vec3 NewNormal;                                                                         
    mat3 TBN = mat3(Tangent, Bitangent, Normal);                                            
    NewNormal = TBN * BumpMapNormal;                                                        
    NewNormal = normalize(NewNormal);                                                       
    return NewNormal;                                                                       
}     

void main()
{
    vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) *
                        gDirectionalLight.AmbientIntensity *
                        vec4(gMaterial.AmbientColor, 1.0f);

    vec3 Normal = normalize(CalcBumpedNormal());

    float DiffuseFactor = dot(Normal, -gDirectionalLight.Direction);

    vec4 DiffuseColor = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) *
                       gDirectionalLight.DiffuseIntensity *
                       vec4(gMaterial.DiffuseColor, 1.0f) *
                       DiffuseFactor;

        vec3 PixelToCamera = normalize(gCameraLocalPos - LocalPos0);
        vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));
        float SpecularFactor = dot(PixelToCamera, LightReflect);
        if (SpecularFactor > 0) {
            float Roughness = texture2D(gSamplerSpecularExponent, TexCoord0).r;
            float SpecularExponent = mix(32.0, 4.0, Roughness);
            SpecularFactor = pow(SpecularFactor, SpecularExponent);
            SpecularColor = vec4(gDirectionalLight.Color, 1.0f) *
                            vec4(gMaterial.SpecularColor, 1.0f) *
                            SpecularFactor * 0.5f;
        }
    }

    vec4 TexColor = texture2D(gSampler, TexCoord0.xy);

    FragColor = TexColor * (AmbientColor + DiffuseColor) +
                SpecularColor;
}