#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 600;

float vertices[] = {
    -0.5f, 0.0f,  0.5f,     0.5f,  0.0f,  0.5f,	0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
    0.5f, 0.0f, -0.5f,     0.0f,  0.5f,  0.5f,	0.0f, 0.0f,
    0.5f, 0.0f,  0.5f,     1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
    0.0f, 0.8f,  0.0f,     0.0f,  0.5f,  0.5f,	0.5f, 1.0f
};

unsigned int indices[] = {
    0,1,2,
    0,2,3,
    0,1,4,
    1,2,4,
    2,3,4,
    3,0,4
};

float cubeVertices[] = {
    // Posiciones          // Colores           // Coordenadas de textura
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f
};

unsigned int cubeIndices[] = {
    0, 1, 2,  2, 3, 0, // Cara frontal
    4, 5, 6,  6, 7, 4, // Cara trasera
    0, 4, 7,  7, 3, 0, // Cara izquierda
    1, 5, 6,  6, 2, 1, // Cara derecha
    0, 1, 5,  5, 4, 0, // Cara inferior
    3, 2, 6,  6, 7, 3  // Cara superior
};

// Código fuente de shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTextCoord;\n"
"out vec3 ourColor;\n"
"out vec2 textCoord;\n"
"uniform mat4 CameraMatrix;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"gl_Position = CameraMatrix * model * vec4(aPos, 1.0);\n"
"ourColor = aColor;\n"
"textCoord = aTextCoord;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 textCoord;\n"
"uniform sampler2D text0;\n"
"void main()\n"
"{\n"
"FragColor = texture(text0, textCoord);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Computacion Grafica", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create the window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Crear la pirámide
    Mesh pyramid(vertices, sizeof(vertices), indices, sizeof(indices), "guy.jpg");

    // Crear el cubo
    Mesh cube(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices), "brickwall.jpg");

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activar el programa de shaders
        glUseProgram(shaderProgram);

        // Configurar la matriz de la cámara
        camera.CameraMatrix(45.0f, 0.1f, 100.0f, shaderProgram, "CameraMatrix");
        camera.CameraInputs(window);

        // Dibujar la pirámide
        glm::mat4 pyramidModel = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
        pyramid.draw();

        // Dibujar el cubo
        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, glm::vec3(2.0f, 1.0f, 0.0f)); // Posición del cubo
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
        cube.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}