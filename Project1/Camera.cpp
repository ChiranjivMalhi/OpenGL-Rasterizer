#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>

using namespace std;

static float ToDegree(float r) { return glm::degrees(r); }

static glm::vec3 rotateVec(const glm::vec3& v, float angleDegrees, const glm::vec3& axis)
{
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis);
	glm::vec4 res = R * glm::vec4(v, 0.0f);
	return glm::normalize(glm::vec3(res));
}

Camera::Camera()
	: Position(0.0f, 0.0f, 5.0f)
	, Front(0.0f, 0.0f, -1.0f)
	, WorldUp(0.0f, 1.0f, 0.0f)
	, Yaw(-90.0f)
	, Pitch(0.0f)
	, MovementSpeed(3.0f)
	, MouseSensitivity(0.1f)
	, m_windowWidth(1280)
	, m_windowHeight(720)
{
	updateCameraVectors();
	m_target = Front;
	m_up = Up;
	m_AngleH = Yaw;
	m_AngleV = Pitch;
	m_mousePos = glm::ivec2(m_windowWidth / 2, m_windowHeight / 2);
}

Camera::Camera(const int m_windowWidth, const int m_windowHeight, const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
	: Position(position)
	, Front(0.0f, 0.0f, -1.0f)
	, WorldUp(up)
	, Yaw(yaw)
	, Pitch(pitch)
	, MovementSpeed(3.0f)
	, MouseSensitivity(0.1f)
	, m_windowWidth(m_windowWidth)
	, m_windowHeight(m_windowHeight)
{
	updateCameraVectors();
	m_target = Front;
	m_up = Up;
	m_AngleH = Yaw;
	m_AngleV = Pitch;
	m_mousePos = glm::ivec2(m_windowWidth / 2, m_windowHeight / 2);
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Init()
{
	glm::vec3 HTarget(m_target.x, 0.0f, m_target.z);
	if (glm::length(HTarget) > 0.0001f) HTarget = glm::normalize(HTarget);

	float Angle = ToDegree(asin(fabs(HTarget.z)));

	if (HTarget.z >= 0.0f)
	{
		if (HTarget.x >= 0.0f) m_AngleH = 360.0f - Angle;
		else m_AngleH = 180.0f + Angle;
	}
	else
	{
		if (HTarget.x >= 0.0f) m_AngleH = Angle;
		else m_AngleH = 180.0f - Angle;
	}

	m_AngleV = -ToDegree(asin(m_target.y));


	m_mousePos.x  = m_windowWidth / 2;
	m_mousePos.y  = m_windowHeight / 2;
}

void Camera::OnMouse(int x, int y, GLFWwindow* window)
{
	float DeltaX = x - m_mousePos.x;
	float DeltaY = y - m_mousePos.y;
	float DeltaZ = glm::abs(Position[2]);

	if (DeltaZ > 75.0f) DeltaZ = 75.0f;

	MovementSpeed = 3.0f + DeltaZ / 5.0f;

	m_mousePos.x = x;
	m_mousePos.y = y;

	if (!getMMBState() && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		m_AngleH -= (float)DeltaX / 20.0f;
		m_AngleV += (float)DeltaY / 50.0f;
	}


	if (getMMBState()) {

		float panSpeed = 0.01f*DeltaZ;

		Position -= Right * DeltaX * panSpeed;
		Position += Up * DeltaY * panSpeed;
	}
		

	Update();
}

void Camera::OnScroll(float offset)
{
	float zoomSpeed = 0.5f;

	Position += Front * offset * zoomSpeed;
}


void Camera::Update()
{
	glm::vec3 Yaxis(0.0f, 1.0f, 0.0f);

	glm::vec3 View(1.0f, 0.0f, 0.0f);
	View = rotateVec(View, m_AngleH, Yaxis);
	View = glm::normalize(View);

	glm::vec3 U = glm::cross(Yaxis, View);
	if (glm::length(U) > 0.0001f) U = glm::normalize(U);
	View = rotateVec(View, m_AngleV, U);

	m_target = glm::normalize(View);

	m_up = glm::cross(m_target, U);
	if (glm::length(m_up) > 0.0001f) m_up = glm::normalize(m_up);

	Front = m_target;
	Up = m_up;
	Right = glm::normalize(glm::cross(Front, WorldUp));
}


void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += Front * velocity;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position -= Front * velocity;
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position += Right * velocity;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position -= Right * velocity;
	
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		Position += WorldUp * velocity;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		Position -= WorldUp * velocity;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		MMBPressed();
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
		MMBReleased();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void Camera::updateCameraVectors()
{
	
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
