#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>
#include "Shader.h"

class ShaderProgram {
public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    void use() const;
    GLuint getUniformLocation(const char* name) const;

private:
    GLuint id;
    void checkLinkErrors();
};

#endif

