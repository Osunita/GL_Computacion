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
    // Matriz de transformaci�n para mover, rotar y escalar la malla
    glm::mat4 model;

    // Buffers de la malla
    VAO vao;
    VBO* vbo;
    EBO* ebo;
    GLsizei numIndices; // N�mero de �ndices para dibujar

    // Constructor: se reciben los v�rtices e �ndices en vectores
    // Se asume que cada v�rtice tiene 11 floats:
    // [Posici�n (3) | Color (3) | TexCoords (2) | Normal (3)]
    Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);

    // Destructor
    ~Mesh();

    // M�todos de transformaci�n
    void Translate(const glm::vec3& translation);
    void Rotate(float angle, const glm::vec3& axis);
    void Scale(const glm::vec3& scaleVec);

    // M�todo para dibujar la malla usando un shader
    void Draw(Shader& shader);
};

#endif // MESH_H
