#include "shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vertexSource = getShaderSource(vertexPath);
    std::string fragmentSource = getShaderSource(fragmentPath);

    unsigned int vertexShader = compileShader(vertexSource.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);

    int success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        throw std::runtime_error("Program linking failed: " + std::string(infoLog));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::use() const
{
    glUseProgram(m_id);
}

unsigned int Shader::compileShader(const char *source, GLenum type)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        throw std::runtime_error("Shader compilation failed: " + std::string(infoLog));
    }

    return shader;
}

std::string Shader::getShaderSource(const char *path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + std::string(path));
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceStr = buffer.str();
    file.close();

    return sourceStr;
}

int Shader::getUniformLocation(const std::string &name)
{
    auto it = m_uniformLocations.find(name);
    if (it != m_uniformLocations.end())
    {
        return it->second;
    }

    int location = glGetUniformLocation(m_id, name.c_str());
    m_uniformLocations[name] = location;
    return location;
}
