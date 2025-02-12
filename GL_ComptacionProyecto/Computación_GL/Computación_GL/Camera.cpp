#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;

	lastX = width / 2.0f;
	lastY = height / 2.0f;
}

void Camera::CameraMatrix(float FOV, float nearPlane, float farPlane, unsigned int ShadeID, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);

	projection = glm::perspective(FOV, (float)(width / height), nearPlane, farPlane);

	GLuint CameraUniform = glGetUniformLocation(ShadeID, "CameraMatrix");
	glUniformMatrix4fv(CameraUniform, 1, GL_FALSE, glm::value_ptr(projection * view));

}

void Camera::CameraInputs(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_W)){
		Position += speed * Orientation;
	}
	if(glfwGetKey(window, GLFW_KEY_A)){
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if(glfwGetKey(window, GLFW_KEY_S)){
		Position += speed * -Orientation;
	}
	if(glfwGetKey(window, GLFW_KEY_D)){
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}

	if (glfwGetKey(window, GLFW_KEY_Q)) {
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_R)) {
		Position += speed * -Up;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
		speed = 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
		speed = 0.1f;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		ProcessMouseMovement(xpos, ypos);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		firstMouse = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Camera::ProcessMouseMovement(double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	xOffset *= sensitivity;
	yOffset *= sensitivity;

	glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
	Orientation = glm::rotate(Orientation, glm::radians(-xOffset), Up);
	Orientation = glm::rotate(Orientation, glm::radians(-yOffset), right);
	Orientation = glm::normalize(Orientation);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Orientation, Up);
}
	
