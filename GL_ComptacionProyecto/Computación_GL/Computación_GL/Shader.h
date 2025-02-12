#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader {
public:
    Shader(const char* source, GLenum shaderType);
    ~Shader();

    GLuint getId() const;

private:
    GLuint id;
    void checkCompileErrors(GLuint shader, const std::string& type);
};

#endif