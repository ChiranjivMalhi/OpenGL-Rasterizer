#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "world_transform.h"
#include "BasicMesh.h"
#include <fstream>
#include <sstream>
#include <string>
#include "lighting_technique.h"
#include "common.h"
#include <vector>
#include <memory>

using namespace std;

#define MODEL_PATH "../Content/"
#define DEFAULT_MODEL MODEL_PATH "small_lpg_tank_4k.fbx"

class Application {
public:
    Application(int width = 1280, int height = 720)
        : m_window(nullptr)
        , m_width(width)
        , m_height(height)
        , m_translationLocation(-1)
        , m_pMesh(nullptr)
    {
        
    
    }

    ~Application();

    bool Init();
    void Run();

private:
    GLint m_translationLocation;

    GLFWwindow* m_window;
    int m_width;
    int m_height;

    Camera m_camera;
    WorldTransform m_world;
    BasicMesh* m_pMesh;

    LightingTechnique* pLightingTech = NULL;
    DirectionalLight dirLight;

    void RenderScene();

    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

Application::~Application()
{
    if (m_pMesh) {
        delete m_pMesh;
        m_pMesh = nullptr;
    }
}

void Application::RenderScene()
{
	glm::mat4 T(1.0f);
	glm::mat4 R(1.0f);
	R = glm::rotate(R, -1.5708f, glm::vec3(1, 0, 0));

	glm::mat4 R2(1.0);
	float angle = (float)glfwGetTime() * 0.5f;
	R2 = glm::rotate(R2, angle, glm::vec3(0, 1, 0));
	glm::mat4 Mmodel = T * R2 * R;

	glm::mat4 MVP = m_world.getMVP(m_camera);
	MVP = MVP * Mmodel;

    glUniformMatrix4fv(m_translationLocation, 1, GL_FALSE, &MVP[0][0]);

    dirLight.CalcLocalDirection(Mmodel);

    pLightingTech->SetWVP(MVP);
    pLightingTech->SetDirectionalLight(dirLight);
    pLightingTech->SetMaterial(m_pMesh->GetMaterial());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 CameraToLocalTransformation = glm::inverse(Mmodel);

    glm::vec4 CameraWorldPos = glm::vec4(m_camera.getPosition(), 1.0f);

    glm::vec4 CameraLocalPos = CameraToLocalTransformation * CameraWorldPos;

    glm::vec3 CameraLocalPos3f = glm::vec3(CameraLocalPos);

    pLightingTech->SetCameraLocalPos(CameraLocalPos3f);

    if (m_pMesh) {
        m_pMesh->Render();
    }
}

void Application::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->m_camera.OnMouse((int)xpos, (int)ypos, window);
    }
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->m_camera.OnScroll((float)yoffset);
    }
}

bool Application::Init()
{
    if (!glfwInit())
        return false;

    m_window = glfwCreateWindow(m_width, m_height, "Hello World", NULL, NULL);
    if (!m_window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if (glewInit() != GLEW_OK) {
        cerr << "GLEW init error" << endl;
        return false;
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    dirLight.WorldDirection = glm::vec3(1.0f, 0.0, 0.0);
	dirLight.Color = glm::vec3(1.0f);
    dirLight.DiffuseIntensity = 1.0f;
    dirLight.AmbientIntensity = 0.2f;


    pLightingTech = new LightingTechnique();

    if (!pLightingTech->Init())
    {
        return false;
    }

    pLightingTech->Enable();

    pLightingTech->SetTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	pLightingTech->SetSpecularExponentTextureUnit(SPECULAR_EXPONENT_UNIT_INDEX);
	pLightingTech->SetNormalMapTextureUnit(NORMAL_TEXTURE_UNIT_INDEX);

    //CompileShaders();

    m_pMesh = new BasicMesh();
    if (!m_pMesh->LoadMesh(DEFAULT_MODEL)) {
        cerr << "Failed to load mesh: " << DEFAULT_MODEL << endl;
        cerr << "Make sure the file exists at: " << DEFAULT_MODEL << endl;
        cerr << "Model path placeholder: " << MODEL_PATH << endl;
        return false;
    }

    m_world.setProjection(45.0f, (float)m_width / (float)m_height, 0.1f, 100000.0f);

    glfwSetCursorPos(m_window, (double)m_width / 2.0, (double)m_height / 2.0);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetCursorPosCallback(m_window, CursorPosCallback);
    glfwSetScrollCallback(m_window, ScrollCallback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_camera.Init();

    return true;
}

void Application::Run()
{
    float lastFrame = (float)glfwGetTime();

    glfwWindowHint(GLFW_SAMPLES, 4);

    while (!glfwWindowShouldClose(m_window)) {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        m_camera.processKeyboard(m_window, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_Z) == GLFW_PRESS) {
            float s = m_camera.getSpeed();
            s = std::max(0.1f, s - 1.0f * deltaTime);
            m_camera.setSpeed(s);
        }
        if (glfwGetKey(m_window, GLFW_KEY_X) == GLFW_PRESS) {
            float s = m_camera.getSpeed();
            s = s + 1.0f * deltaTime;
            m_camera.setSpeed(s);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        RenderScene();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();
}

int main(int argc, char** argv)
{
    Application app(1280, 720);
    if (!app.Init()) {
        return -1;
    }
    app.Run();
    return 0;
}
