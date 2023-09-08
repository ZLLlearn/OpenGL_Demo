#include <iostream>
#include <fstream>
#include <sstream>
#include "glad/glad.h"

#include "shader.h"


Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if (geometryPath) {
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    }
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (geometryPath) {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure& e) {
        std::cout << "Error ShaderFile not successfully read : " << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    uint32_t geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(vertex);
    glCompileShader(fragment);
    checkCompileErrors(vertex, "Vertex");
    checkCompileErrors(fragment, "Fragment");
    if (geometryPath) {
        const char* gShaderCode = geometryCode.c_str();
        glShaderSource(geometry, 1, &gShaderCode, nullptr);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "Geometry");
    }
    
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    if (geometryPath) {
        glAttachShader(m_id, geometry);
    }
    glLinkProgram(m_id);
    checkCompileErrors(m_id, "Program");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath) {
        glDeleteShader(geometry);
    }
}

Shader::~Shader() {
    glDeleteProgram(m_id);
}

void Shader::activate() {
    glUseProgram(m_id);
}

void Shader::setBool(const std::string& name, bool value) {
    setInt(name, static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setVec(const std::string& name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec(const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec(const std::string& name, float x, float y) {
    glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
}

void Shader::setVec(const std::string& name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
}

void Shader::setVec(const std::string& name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
}

void Shader::setMat(const std::string& name, const glm::mat2& mat) {
    glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat(const std::string& name, const glm::mat3& mat) {
    glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat(const std::string& name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(uint32_t stage, const std::string& type) {
    int success = 1;
    char infoLog[1024];
    if (type != "Program") {
        glGetShaderiv(stage, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(stage, 1024, nullptr, infoLog);
            std::cout << "Shader compile_compilation_error of type : " << type << "\n" << infoLog << std::endl;
        }
    }
    else {
        glGetProgramiv(stage, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(stage, 1024, nullptr, infoLog);
            std::cout << "Shader program_linking_error of type : " << type << "\n" << infoLog << std::endl;
        }
    }
}
