#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <string>
#include <vector>

class Mesh {
public:
    // Constructor
    Mesh(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize, const std::string& texturePath);

    // Dibuja la malla
    void draw();

private:
    // IDs de OpenGL
    unsigned int VAO, VBO, IBO, textureID;

    // Configura los buffers de OpenGL
    void setupMesh(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize);

    // Carga una textura desde un archivo
    void loadTexture(const std::string& texturePath);
};

#endif