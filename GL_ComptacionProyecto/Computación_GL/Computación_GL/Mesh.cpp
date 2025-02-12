// Mesh.cpp
#include "Mesh.h"
#include <stb/stb_image.h>
#include <iostream>

Mesh::Mesh(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize, const std::string& texturePath) {
    // Configurar los buffers de la malla
    setupMesh(vertices, verticesSize, indices, indicesSize);

    // Cargar la textura
    loadTexture(texturePath);
}

void Mesh::setupMesh(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize) {
    // Generar y configurar VAO, VBO e IBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    // Configurar VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    // Configurar IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    // Atributos de vértice
    unsigned int stride = 8 * sizeof(float); // 3 (posición) + 3 (color) + 2 (textura)

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Coordenadas de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    //EnableVertexAttribArray(3);

    // Desvincular VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::loadTexture(const std::string& texturePath) {
    // Cargar la imagen
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        // Generar y configurar la textura
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Cargar los datos de la imagen en la textura
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Liberar los datos de la imagen
        stbi_image_free(data);
    }
    else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
}

void Mesh::draw() {
    // Activar la textura
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Dibujar la malla
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Ajusta el número de índices si es necesario
    glBindVertexArray(0);
}