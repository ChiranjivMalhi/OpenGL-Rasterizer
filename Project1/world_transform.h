#pragma once

#include <glm/glm.hpp>
#include "Camera.h"

class WorldTransform {
public:
	WorldTransform();

	void setProjection(float fovDegrees, float aspect, float zNear, float zFar);
	void setModelTranslation(const glm::vec3& t) { modelTranslation = t; }
	void setModelRotation(const glm::vec3& r) { modelRotation = r; }

	glm::mat4 getMVP(const Camera& camera) const;

private:
	float FOV;
	float Aspect;
	float ZNear;
	float ZFar;

	glm::vec3 modelTranslation;
	glm::vec3 modelRotation; // Euler angles in radians
};
