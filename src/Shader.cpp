#include "Shader.h"
#include "Renderer.h"

#include <fstream>
#include <glad/glad.h>
#include <sstream>

#include <GLFW/glfw3.h>
#include <iostream>

Shader::Shader(const std::string &filepath)
    : m_FilePath(filepath), m_RendereID(0) {
    ShaderProgramSource source = ParseShader(filepath);
    m_RendereID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() { GLCall(glDeleteProgram(m_RendereID)); }
void Shader::Bind() const { GLCall(glUseProgram(m_RendereID)); }
void Shader::UnBind() const { GLCall(glUseProgram(0)); }

void Shader::SetUniform1i(const std::string &name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
};

void Shader::SetUniform2i(const std::string &name, int v0, int v1) {
    GLCall(glUniform2i(GetUniformLocation(name), v0, v1));
};

void Shader::SetUniform1f(const std::string &name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
};

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2,
                          float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,
                              &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string &name) {

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    GLCall(int location = glGetUniformLocation(m_RendereID, name.c_str()));

    if (location == -1)
        std::cout << "Warning " << name << " doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::ParseShader(const std::string &filepath = "") {
    std::ifstream stream(filepath);

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

// Compile the shader and return the shader id
unsigned int Shader::CompileShader(unsigned int type,
                                   const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src,
                   nullptr); // Gives GLSL source code to the shader
    glCompileShader(id);     // Compile GLSL code

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // verify if shader compiled
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader!"
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader,
                                  const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // Free OpenGL ressources
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
