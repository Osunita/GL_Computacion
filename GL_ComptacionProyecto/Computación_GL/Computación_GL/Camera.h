#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	int width;
	int height;
	float speed;

	float sensitivity = 0.1f;
	bool firstMouse = true;
	float lastX;
	float lastY;

	Camera(int width, int height, glm::vec3 position);

	void CameraMatrix(float FOV, float nearPlane, float farPlane, unsigned int ShadeID, const char* uniform);

	void CameraInputs(GLFWwindow* window);

	void ProcessMouseMovement(double xpos, double ypos);

	glm::mat4 GetViewMatrix();

};

