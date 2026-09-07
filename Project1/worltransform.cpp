#include "world_transform.h"
#include <glm/gtc/matrix_transform.hpp>

WorldTransform::WorldTransform()
	: FOV(45.0f)
	, Aspect(4.0f/3.0f)
	, ZNear(0.0f)
	, ZFar(1000000000.0f)
	, modelTranslation(0.0f)
	, modelRotation(0.0f)
{
}

void WorldTransform::setProjection(float fovDegrees, float aspect, float zNear, float zFar)
{
	FOV = fovDegrees;
	Aspect = aspect;
	ZNear = zNear;
	ZFar = zFar;
}

glm::mat4 WorldTransform::getMVP(const Camera& camera) const
{
	glm::mat4 P = glm::perspective(glm::radians(FOV), Aspect, ZNear, ZFar);

	glm::mat4 V = camera.getViewMatrix();

	glm::mat4 M(1.0f);
	M = glm::translate(M, modelTranslation);
	M = glm::rotate(M, modelRotation.x, glm::vec3(1,0,0));
	M = glm::rotate(M, modelRotation.y, glm::vec3(0,1,0));
	M = glm::rotate(M, modelRotation.z, glm::vec3(0,0,1));

	return P * V * M;
}
