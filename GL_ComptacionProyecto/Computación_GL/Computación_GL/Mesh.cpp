#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Constructor: inicializa la malla, creando y configurando los buffers
Mesh::Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
    : model(glm::mat4(1.0f)), numIndices(static_cast<GLsizei>(indices.size()))
{
    vao.Bind();
    vbo = new VBO(const_cast<GLfloat*>(vertices.data()), static_cast<GLsizeiptr>(vertices.size() * sizeof(GLfloat)));
    ebo = new EBO(const_cast<GLuint*>(indices.data()), static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)));

    // Cada vértice tiene 11 floats: 3 (posición) + 3 (color) + 2 (texcoords) + 3 (normales)
    GLsizei stride = 11 * sizeof(GLfloat);
    vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, stride, (void*)0);                           // Posición
    vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, stride, (void*)(3 * sizeof(GLfloat)));         // Color
    vao.LinkAttrib(*vbo, 2, 2, GL_FLOAT, stride, (void*)(6 * sizeof(GLfloat)));         // TexCoords
    vao.LinkAttrib(*vbo, 3, 3, GL_FLOAT, stride, (void*)(8 * sizeof(GLfloat)));         // Normales

    vao.Unbind();
    vbo->Unbind();
    ebo->Unbind();
}

// Destructor: liberar los recursos asignados dinámicamente
Mesh::~Mesh() {
    delete vbo;
    delete ebo;
}

// Métodos de transformación que modifican la matriz 'model'
void Mesh::Translate(const glm::vec3& translation) {
    model = glm::translate(model, translation);
}

void Mesh::Rotate(float angle, const glm::vec3& axis) {
    model = glm::rotate(model, glm::radians(angle), axis);
}

void Mesh::Scale(const glm::vec3& scaleVec) {
    model = glm::scale(model, scaleVec);
}

// Método para dibujar la malla
void Mesh::Draw(Shader& shader) {
    shader.Activate();
    // Enviar la matriz de modelo al shader (se asume que el uniform se llama "model")
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    vao.Bind();
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}
