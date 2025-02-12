#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) {
    id = glCreateProgram();
    glAttachShader(id, vertexShader.getId());
    glAttachShader(id, fragmentShader.getId());
    glLinkProgram(id);

    checkLinkErrors();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

void ShaderProgram::use() const {
    glUseProgram(id);
}

GLuint ShaderProgram::getUniformLocation(const char* name) const {
    return glGetUniformLocation(id, name);
}

void ShaderProgram::checkLinkErrors() {
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 1024, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n";
    }
}
