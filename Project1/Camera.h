#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
	Camera();
	Camera(const int m_windowWidth, const int m_windowHeight, const glm::vec3& position, const glm::vec3& up, float yaw, float pitch);

	glm::mat4 getViewMatrix() const;

	void Init();
	void OnMouse(int x, int y, GLFWwindow* window);
	void OnScroll(float offset);
	void Update();

	void processKeyboard(GLFWwindow* window, float deltaTime);

	void setSpeed(float s) { MovementSpeed = s; }
	float getSpeed() const { return MovementSpeed; }

	void MMBPressed() { isMMB = true; }
	void MMBReleased() { isMMB = false; }

	bool getMMBState() const { return isMMB; }

	glm::vec3 getPosition() const { return Position; }

private:
	void updateCameraVectors();

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	glm::vec3 m_target;
	glm::vec3 m_up;

	int m_windowWidth;
	int m_windowHeight;

	float Yaw;
	float Pitch;

	float m_AngleH;
	float m_AngleV;

	bool isMMB;

	glm::ivec2 m_mousePos;

	float MovementSpeed;
	float MouseSensitivity;
};
