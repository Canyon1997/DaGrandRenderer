#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "Shader.h"
#include <string>
#include "Texture.h"

// Math Libraries
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Window
#include <windows.h>
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

// World Variables
const glm::vec3 WORLD_UP_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraForwardDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraForwardDir, cameraUp));

float fov = 45.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float lastMouseX = static_cast<float>(WINDOW_WIDTH) / 2.0f;
float lastMouseY = static_cast<float>(WINDOW_HEIGHT) / 2.0f;
bool firstFrame = true;
void RotateCamera(const double& xPos, const double& yPos);
glm::mat4 customLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

// global light source
glm::vec3 lightPos(0.0f, 1.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

// delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// init functions
bool initGlfw();
bool initGlad();

// window functions
GLFWwindow* createGlfwWindow(int width, int height, const std::string& windowName);

// input
void ProcessInput(GLFWwindow* currentWindow);



int main(int argc, char* argv[])
{
    // init system
    initGlfw();
    GLFWwindow* mainWindow = createGlfwWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "DaGrandRenderer");
    glfwMakeContextCurrent(mainWindow);
    if (!initGlad())
    {
        return -1;
    }

    // set current viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(mainWindow, [](GLFWwindow*, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    // Callbacks
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mainWindow, [](GLFWwindow* window, double xPos, double yPos)
    {
        RotateCamera(xPos, yPos);
    });

    glfwSetScrollCallback(mainWindow, [](GLFWwindow* window, double xOffset, double yOffset)
    {
       fov -= static_cast<float>(yOffset);
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    });


    // geometry data
    float vertexData[] = {

        // positions          //normals perpendicular to surface position
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

    unsigned int VBO1;
    unsigned int VAO1;

    // Vertex array and buffer definitions
    glGenBuffers(1, &VBO1);
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int VBO2;
    unsigned int VAO2;

    glGenBuffers(1, &VBO2);
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // shaders
    Shader::flipVerticallyOnLoad(true);

    Shader objectShader("vObject.glsl", "fObject.glsl");
    Shader lightSourceShader("vLightSource.glsl", "fLightSource.glsl");

    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(mainWindow))
    {
        // set delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        ProcessInput(mainWindow);

        // clear buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // render model 1
        objectShader.use();
        glUniform3f(glGetUniformLocation(objectShader.getProgram(), "objectColor"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(objectShader.getProgram(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);

        glBindVertexArray(VAO1);

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(objectShader.getProgram(), "model"), 1, false, glm::value_ptr(model));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraForwardDir, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(objectShader.getProgram(), "view"), 1, false, glm::value_ptr(view));

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(view * model));
        glUniformMatrix3fv(glGetUniformLocation(objectShader.getProgram(), "normalMatrix"), 1, false, glm::value_ptr(normalMatrix));

        glm::vec3 viewLightPos = glm::vec3(view * glm::vec4(lightPos, 1.0));
        glUniform3f(glGetUniformLocation(objectShader.getProgram(), "lightSourcePos"), viewLightPos.x, viewLightPos.y, viewLightPos.z);

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(objectShader.getProgram(), "proj"), 1, false, glm::value_ptr(projection));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // render model 2
        lightSourceShader.use();
        glUniform3f(glGetUniformLocation(lightSourceShader.getProgram(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);

        glBindVertexArray(VAO2);

        glm::mat4 lightModel = glm::mat4(1.0f);
        lightPos.x = 2.0f * sin(glfwGetTime());
        lightPos.z = 2.0f * cos(glfwGetTime());
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.getProgram(), "model"), 1, false, glm::value_ptr(lightModel));

        glm::mat4 lightView = glm::mat4(1.0f);
        lightView = glm::lookAt(cameraPos, cameraPos + cameraForwardDir, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.getProgram(), "view"), 1, false, glm::value_ptr(lightView));

        glm::mat4 lightProjection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.getProgram(), "proj"), 1, false, glm::value_ptr(lightProjection));

        glDrawArrays(GL_TRIANGLES, 0, 36);


        // check/call events, swap buffers, etc.
        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void ProcessInput(GLFWwindow* currentWindow)
{
    // Camera inputs
    float cameraSpeed = 3.0f * deltaTime;
    if (glfwGetKey(currentWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraForwardDir;
    }
    if (glfwGetKey(currentWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraForwardDir;
    }
    if (glfwGetKey(currentWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraForwardDir, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(currentWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraForwardDir, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(currentWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraRight, cameraForwardDir)) * cameraSpeed;
    }
    if (glfwGetKey(currentWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraRight, cameraForwardDir)) * cameraSpeed;
    }

    // Exit input
    if (glfwGetKey(currentWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(currentWindow, GLFW_TRUE);
    }
}

void RotateCamera(const double& xPos, const double& yPos)
{
    // init for first frame so movement doesnt happen initially
    if (firstFrame)
    {
        lastMouseX = xPos;
        lastMouseY = yPos;
        firstFrame = false;
    }

    // set offsets
    float xOffset = static_cast<float>(xPos) - lastMouseX;
    float yOffset = lastMouseY - static_cast<float>(yPos);
    lastMouseX = xPos;
    lastMouseY = yPos;

    // apply delta time influence
    float sensitivity = 15.0f * deltaTime;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    pitch += yOffset;
    yaw += xOffset;

    // constrain rotation values
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    // apply rotations
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // update camera cooridnates
    cameraForwardDir = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraForwardDir, WORLD_UP_VECTOR));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraForwardDir));
}

bool initGlfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

bool initGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

GLFWwindow* createGlfwWindow(int width, int height, const std::string& windowName)
{
    GLFWwindow* window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    return window;
}
