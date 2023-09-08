#ifndef SHADER_H
#define SHADER_H
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    ~Shader();

    void activate();
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec(const std::string& name, const glm::vec2& value);
    void setVec(const std::string& name, const glm::vec3& value);
    void setVec(const std::string& name, const glm::vec4& value);
    void setVec(const std::string& name, float x, float y);
    void setVec(const std::string& name, float x, float y, float z);
    void setVec(const std::string& name, float x, float y, float z, float w);
    void setMat(const std::string& name, const glm::mat2& mat);
    void setMat(const std::string& name, const glm::mat3& mat);
    void setMat(const std::string& name, const glm::mat4& mat);

private:
    void checkCompileErrors(uint32_t stage, const std::string& type);
    uint32_t m_id;

};


#endif