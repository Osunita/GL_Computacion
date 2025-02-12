#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
public:
    // Constructor
    Texture(const std::string& path, const std::string& type = "texture_diffuse");

    // Cargar la textura
    void load();

    // Activar la textura
    void bind(unsigned int unit = 0) const;

    // Obtener el ID de la textura
    unsigned int getID() const;

private:
    unsigned int id;      // ID de la textura
    std::string path;     // Ruta de la textura
    std::string type;     // Tipo de textura (diffuse, specular, etc.)
    int width, height, nrChannels; // Dimensiones y canales de la imagen
};

#endif

