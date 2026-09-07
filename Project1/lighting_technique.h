#include "technique.h"
#include "material.h"
#include <glm/glm.hpp>

class BaseLight
{
public:
    glm::vec3 Color;
    float AmbientIntensity;

    BaseLight()
    {
        Color = glm::vec3(0.0f, 1.0f, 1.0f);
        AmbientIntensity = 0.1f;
    }
};

class DirectionalLight : public BaseLight
{
public:
    glm::vec3 WorldDirection = glm::vec3(0.0f, 0.0f, 0.0f);
    float DiffuseIntensity = 0.0f;

    void CalcLocalDirection(const glm::mat4& World);

    const glm::vec3& GetLocalDirection() const { return LocalDirection; }

private:
    glm::vec3 LocalDirection = glm::vec3(0.0f, 0.0f, 0.0f);
};




class LightingTechnique : public Technique
{
public:

    LightingTechnique();

    virtual bool Init();

    void SetWVP(const glm::mat4& WVP);
    void SetTextureUnit(unsigned int TextureUnit);
    void SetDirectionalLight(const DirectionalLight& Light);
    void SetSpecularExponentTextureUnit(unsigned int TextureUnit);
    void SetCameraLocalPos(const glm::vec3& CameraLocalPos);
    void SetMaterial(const Material& material);
	void SetNormalMapTextureUnit(unsigned int TextureUnit);

private:

    GLuint WVPLoc;
    GLuint samplerLoc;

    GLuint normalMapLoc;

    GLuint samplerSpecularExponentLoc;
    GLuint CameraLocalPosLoc;

    struct {
        GLuint AmbientColor;
        GLuint DiffuseColor;
		GLuint SpecularColor;
    } materialLoc;

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint Direction;
        GLuint DiffuseIntensity;
    } dirLightLoc;
};
