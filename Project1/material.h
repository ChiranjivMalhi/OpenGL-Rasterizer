#ifndef GLRENDERER_MATERIAL_H
#define GLRENDERER_MATERIAL_H

#include<glm/glm.hpp>
#include"texture.h"

class Material {

public:
    glm::vec3 AmbientColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 DiffuseColor = glm::vec3(0.0f, 0.0f, 0.0f); 
	glm::vec3 SpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);

    Texture* pDiffuse = NULL;
    Texture* pSpecularExponent = NULL;
	Texture* pNormal = NULL;
};


#endif
