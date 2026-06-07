#pragma once

#include <glad/gl.h>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use() const;

    inline void setMat4(const std::string& name, const glm::mat4& value) { glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)); }

private:
    unsigned int compileShader(const char* source, GLenum type);
    std::string getShaderSource(const char* path);
    int getUniformLocation(const std::string& name);

private:
    unsigned int m_id;
    std::unordered_map<std::string, int> m_uniformLocations;
};