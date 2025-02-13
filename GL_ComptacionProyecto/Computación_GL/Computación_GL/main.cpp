#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"Texture.h"
#include"Shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"


const unsigned int width = 800;
const unsigned int height = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTex;\n"
"layout (location = 3) in vec3 aNormal;\n"
"out vec3 color;\n"
"out vec2 texCoord;\n"
"out vec3 Normal;\n"
"out vec3 crntPos;\n"
"uniform mat4 camMatrix;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"    crntPos = vec3(model * vec4(aPos, 1.0f));\n"
"    gl_Position = camMatrix * vec4(crntPos, 1.0);\n"
"    color = aColor;\n"
"    texCoord = aTex;\n"
"    Normal = aNormal;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 color;\n"
"in vec2 texCoord;\n"
"in vec3 Normal;\n"
"in vec3 crntPos;\n"
"uniform sampler2D tex0;\n"
"uniform vec4 lightColor;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 camPos;\n"
"void main()\n"
"{\n"
"    float ambient = 0.20f;\n"
"    vec3 normal = normalize(Normal);\n"
"    vec3 lightDirection = normalize(lightPos - crntPos);\n"
"    float diffuse = max(dot(normal, lightDirection), 0.0f);\n"
"    float specularLight = 0.50f;\n"
"    vec3 viewDirection = normalize(camPos - crntPos);\n"
"    vec3 reflectionDirection = reflect(-lightDirection, normal);\n"
"    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);\n"
"    float specular = specAmount * specularLight;\n"
"    FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);\n"
"}\0";

const char* lightFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 lightColor;\n"
"void main()\n"
"{\n"
"    FragColor = lightColor;\n"
"}\0";

const char* lightVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 camMatrix;\n"
"void main()\n"
"{\n"
"    gl_Position = camMatrix * model * vec4(aPos, 1.0f);\n"
"}\0";


GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS      
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f,
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f,
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  
};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	4, 6, 5,
	7, 9, 8,
	10, 12, 11,
	13, 15, 14
};

GLfloat planeVertices[] =
{ //     COORDINATES         /        COLORS          /    TexCoord   /        NORMALS      
	-3.0f, -0.5f,  3.0f,     0.83f, 0.70f, 0.44f,  0.0f, 0.0f,      0.0f, 1.0f, 0.0f, // Vértice 0: Superior izquierda
	 3.0f, -0.5f,  3.0f,     0.83f, 0.70f, 0.44f,	50.0f, 0.0f,      0.0f, 1.0f, 0.0f, // Vértice 1: Superior derecha
	 3.0f, -0.5f, -3.0f,     0.83f, 0.70f, 0.44f,	50.0f, 50.0f,     0.0f, 1.0f, 0.0f, // Vértice 2: Inferior derecha
	-3.0f, -0.5f, -3.0f,     0.83f, 0.70f, 0.44f,	 0.0f, 50.0f,     0.0f, 1.0f, 0.0f  // Vértice 3: Inferior izquierda
};

GLuint planeIndices[] =
{
	0, 1, 2, // Primer triángulo
	0, 2, 3  // Segundo triángulo
};

// 24 vértices: 6 caras x 4 vértices por cara
GLfloat wallVertices[] = {
	// Front face (cara frontal, z = -10)
	// Posición                 Color         TexCoords      Normal
	-3.0f, -1.0f, -3.0f,       1.0f,1.0f,1.0f,  0.0f, 0.0f,    0.0f,  0.0f, 1.0f, // 0: inferior izquierda
	 3.0f, -1.0f, -3.0f,       1.0f,1.0f,1.0f,  1.0f, 0.0f,    0.0f,  0.0f, 1.0f, // 1: inferior derecha
	 3.0f,  1.0f, -3.0f,       1.0f,1.0f,1.0f,  1.0f, 1.0f,    0.0f,  0.0f, 1.0f, // 2: superior derecha
	-3.0f,  1.0f, -3.0f,       1.0f,1.0f,1.0f,  0.0f, 1.0f,    0.0f,  0.0f, 1.0f, // 3: superior izquierda

	// Back face (cara trasera, z = -3.2)
	-3.0f, -1.0f, -3.2f,       1.0f,1.0f,1.0f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f, // 4: inferior izquierda
	 3.0f, -1.0f, -3.2f,       1.0f,1.0f,1.0f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f, // 5: inferior derecha
	 3.0f,  1.0f, -3.2f,       1.0f,1.0f,1.0f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f, // 6: superior derecha
	-3.0f,  1.0f, -3.2f,       1.0f,1.0f,1.0f,  0.0f, 1.0f,    0.0f,  0.0f, -1.0f, // 7: superior izquierda

	// Left face (lado izquierdo, x = -5)
	-3.0f, -1.0f, -3.2f,       1.0f,1.0f,1.0f,  0.0f, 0.0f,    -1.0f, 0.0f, 0.0f, // 8: inferior atrás
	-3.0f, -1.0f, -3.0f,       1.0f,1.0f,1.0f,  1.0f, 0.0f,    -1.0f, 0.0f, 0.0f, // 9: inferior adelante
	-3.0f,  1.0f, -3.0f,       1.0f,1.0f,1.0f,  1.0f, 1.0f,    -1.0f, 0.0f, 0.0f, // 10: superior adelante
	-3.0f,  1.0f, -3.2f,       1.0f,1.0f,1.0f,  0.0f, 1.0f,    -1.0f, 0.0f, 0.0f, // 11: superior atrás

	// Right face (lado derecho, x = 5)
	 3.0f, -1.0f, -3.0f,       1.0f,1.0f,1.0f,  0.0f, 0.0f,     1.0f, 0.0f, 0.0f,  // 12: inferior adelante
	 3.0f, -1.0f, -3.2f,       1.0f,1.0f,1.0f,  1.0f, 0.0f,     1.0f, 0.0f, 0.0f,  // 13: inferior atrás
	 3.0f,  1.0f, -3.2f,       1.0f,1.0f,1.0f,  1.0f, 1.0f,     1.0f, 0.0f, 0.0f,  // 14: superior atrás
	 3.0f,  1.0f, -3.0f,       1.0f,1.0f,1.0f,  0.0f, 1.0f,     1.0f, 0.0f, 0.0f,  // 15: superior adelante

	 // Top face (cara superior, y = 2)
	 -3.0f,  1.0f, -3.0f,       1.0f,1.0f,1.0f,  0.0f, 0.0f,     0.0f, 1.0f, 0.0f,  // 16: frontal izquierda
	  3.0f,  1.0f, -3.0f,       1.0f,1.0f,1.0f,  1.0f, 0.0f,     0.0f, 1.0f, 0.0f,  // 17: frontal derecha
	  3.0f,  1.0f, -3.2f,       1.0f,1.0f,1.0f,  1.0f, 1.0f,     0.0f, 1.0f, 0.0f,  // 18: trasera derecha
	 -3.0f,  1.0f, -3.2f,       1.0f,1.0f,1.0f,  0.0f, 1.0f,     0.0f, 1.0f, 0.0f,  // 19: trasera izquierda

	 // Bottom face (cara inferior, y = -2)
	 -3.0f, -1.0f, -3.2f,       1.0f,1.0f,1.0f,  0.0f, 0.0f,     0.0f, -1.0f, 0.0f, // 20: trasera izquierda
	  3.0f, -1.0f, -3.2f,       1.0f,1.0f,1.0f,  1.0f, 0.0f,     0.0f, -1.0f, 0.0f, // 21: trasera derecha
	  3.0f, -1.0f, -3.0f,       1.0f,1.0f,1.0f,  1.0f, 1.0f,     0.0f, -1.0f, 0.0f, // 22: frontal derecha
	 -3.0f, -1.0f, -3.0f,       1.0f,1.0f,1.0f,  0.0f, 1.0f,     0.0f, -1.0f, 0.0f  // 23: frontal izquierda
};


GLuint wallIndices[] = {
	// Front face
	0, 1, 2,
	0, 2, 3,
	// Back face
	4, 5, 6,
	4, 6, 7,
	// Left face
	8, 9, 10,
	8, 10, 11,
	// Right face
	12, 13, 14,
	12, 14, 15,
	// Top face
	16, 17, 18,
	16, 18, 19,
	// Bottom face
	20, 21, 22,
	20, 22, 23
};


GLfloat lightVertices[] =
{
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Escena Final", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	// Shader de objetos normales
	Shader shaderProgram(vertexShaderSource, fragmentShaderSource);
	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Shader de luz
	Shader lightShader(lightVertexShaderSource, lightFragmentShaderSource);
	VAO lightVAO;
	lightVAO.Bind();
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

		// VAO, VBO y EBO para el plano
	VAO planeVAO;
	planeVAO.Bind();
	VBO planeVBO(planeVertices, sizeof(planeVertices));
	EBO planeEBO(planeIndices, sizeof(planeIndices));

	// Configuramos los atributos: posición (location 0), color (location 1),
	// coordenadas de textura (location 2) y normales (location 3).
	planeVAO.LinkAttrib(planeVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	planeVAO.LinkAttrib(planeVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	planeVAO.LinkAttrib(planeVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	planeVAO.LinkAttrib(planeVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	planeVAO.Unbind();
	planeVBO.Unbind();
	planeEBO.Unbind();

	VAO wallVAO;
	wallVAO.Bind();
	VBO wallVBO(wallVertices, sizeof(wallVertices));
	EBO wallEBO(wallIndices, sizeof(wallIndices));

	// Configuramos los atributos (asumiendo que usas 11 floats por vértice como en los otros objetos)
	wallVAO.LinkAttrib(wallVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);               // Posición
	wallVAO.LinkAttrib(wallVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float))); // Color
	wallVAO.LinkAttrib(wallVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float))); // TexCoords
	wallVAO.LinkAttrib(wallVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float))); // Normales

	wallVAO.Unbind();
	wallVBO.Unbind();
	wallEBO.Unbind();


	glm::vec4 lightColor = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	glm::mat4 floorModel = glm::mat4(1.0f);
	floorModel = glm::translate(floorModel, glm::vec3(0.0f, -1.0f, 0.0f));

	glm::mat4 wallModel = glm::mat4(1.0f);
	wallModel = glm::translate(wallModel, glm::vec3(0.0f, -0.5f, 0.0f));


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Texture brickTex("Textures/wood.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.textureUnit(shaderProgram, "tex0", 0);

	Texture floorTex("Textures/floor.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	floorTex.textureUnit(shaderProgram, "tex0", 0);

	Texture wallTex("Textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	wallTex.textureUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Camara
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shaderProgram, "camMatrix");

		brickTex.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);


		floorTex.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(floorModel));
		planeVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(planeIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		
		wallTex.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(wallModel));
		wallVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(wallIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	planeVAO.Delete();
	planeVBO.Delete();
	planeEBO.Delete();
	wallVAO.Delete();
	wallVBO.Delete();
	wallEBO.Delete();
	wallTex.Delete();

	brickTex.Delete();
	shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}