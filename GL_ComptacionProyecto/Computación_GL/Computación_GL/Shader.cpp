#include "Shader.h"
#include <iostream>

Shader::Shader(const char* source, GLenum shaderType) {
    id = glCreateShader(shaderType);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    checkCompileErrors(id, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
}

Shader::~Shader() {
    glDeleteShader(id);
}

GLuint Shader::getId() const {
    return id;
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
    }
}

