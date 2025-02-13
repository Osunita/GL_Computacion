#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"

class Mesh {
public:
    // Matriz de transformación para mover, rotar y escalar la malla
    glm::mat4 model;

    // Buffers de la malla
    VAO vao;
    VBO* vbo;
    EBO* ebo;
    GLsizei numIndices; // Número de índices para dibujar

    // Constructor: se reciben los vértices e índices en vectores
    // Se asume que cada vértice tiene 11 floats:
    // [Posición (3) | Color (3) | TexCoords (2) | Normal (3)]
    Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);

    // Destructor
    ~Mesh();

    // Métodos de transformación
    void Translate(const glm::vec3& translation);
    void Rotate(float angle, const glm::vec3& axis);
    void Scale(const glm::vec3& scaleVec);

    // Método para dibujar la malla usando un shader
    void Draw(Shader& shader);
};

#endif // MESH_H
