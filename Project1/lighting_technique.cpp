#include "lighting_technique.h"
#include <iostream>

LightingTechnique::LightingTechnique()
{
}

void DirectionalLight::CalcLocalDirection(const glm::mat4& World)
{
    glm::mat3 World3f(World);  
    glm::mat3 WorldToLocal = glm::transpose(World3f);

    LocalDirection = WorldToLocal * WorldDirection;

    LocalDirection = glm::normalize(LocalDirection);
}

bool LightingTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "shader.vs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "shader.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    WVPLoc = GetUniformLocation("gWVP");
    samplerLoc = GetUniformLocation("gSampler");
    materialLoc.AmbientColor = GetUniformLocation("gMaterial.AmbientColor");
    materialLoc.DiffuseColor = GetUniformLocation("gMaterial.DiffuseColor");
    materialLoc.SpecularColor = GetUniformLocation("gMaterial.SpecularColor");
    dirLightLoc.Color = GetUniformLocation("gDirectionalLight.Color");
    dirLightLoc.AmbientIntensity = GetUniformLocation("gDirectionalLight.AmbientIntensity");
    dirLightLoc.Direction = GetUniformLocation("gDirectionalLight.Direction");
    dirLightLoc.DiffuseIntensity = GetUniformLocation("gDirectionalLight.DiffuseIntensity");
    samplerSpecularExponentLoc = GetUniformLocation("gSamplerSpecularExponent");
    CameraLocalPosLoc = GetUniformLocation("gCameraLocalPos");
    normalMapLoc = GetUniformLocation("gNormalMap");


    if (WVPLoc == 0xFFFFFFFF ||
        samplerLoc == 0xFFFFFFFF ||
        samplerSpecularExponentLoc == 0xFFFFFFFF ||
        materialLoc.AmbientColor == 0xFFFFFFFF ||
        materialLoc.DiffuseColor == 0xFFFFFFFF ||
        materialLoc.SpecularColor == 0xFFFFFFFF ||
        CameraLocalPosLoc == 0xFFFFFFFF ||
        dirLightLoc.Color == 0xFFFFFFFF ||
        dirLightLoc.DiffuseIntensity == 0xFFFFFFFF ||
        dirLightLoc.Direction == 0xFFFFFFFF ||
        dirLightLoc.AmbientIntensity == 0xFFFFFFFF)
    {
        return false;
    }

    return true;
}

void LightingTechnique::SetWVP(const glm::mat4 &WVP)
{
    glUniformMatrix4fv(WVPLoc, 1, GL_FALSE, &WVP[0][0]);
}


void LightingTechnique::SetTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(samplerLoc, TextureUnit);
}


void LightingTechnique::SetSpecularExponentTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(samplerSpecularExponentLoc, TextureUnit);
}

void LightingTechnique::SetCameraLocalPos(const glm::vec3& CameraLocalPos)
{
    glUniform3f(CameraLocalPosLoc, CameraLocalPos.x, CameraLocalPos.y, CameraLocalPos.z);
}

void LightingTechnique::SetNormalMapTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(normalMapLoc, TextureUnit);
}



void LightingTechnique::SetDirectionalLight(const DirectionalLight& Light)
{
    glUniform3f(dirLightLoc.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(dirLightLoc.AmbientIntensity, Light.AmbientIntensity);
    glm::vec3 LocalDirection = Light.GetLocalDirection();
    glUniform3f(dirLightLoc.Direction, LocalDirection.x, LocalDirection.y, LocalDirection.z);
    glUniform1f(dirLightLoc.DiffuseIntensity, Light.DiffuseIntensity);
}


void LightingTechnique::SetMaterial(const Material& material)
{
    glUniform3f(materialLoc.AmbientColor, material.AmbientColor.r, material.AmbientColor.g, material.AmbientColor.b);
    glUniform3f(materialLoc.DiffuseColor, material.DiffuseColor.r, material.DiffuseColor.g, material.DiffuseColor.b);
	glUniform3f(materialLoc.SpecularColor, material.SpecularColor.r, material.SpecularColor.g, material.SpecularColor.b);
}