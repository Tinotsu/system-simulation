#pragma once

#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
  private:
    std::string m_FilePath;
    unsigned int m_RendereID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

  public:
    Shader(const std::string &filepath);
    ~Shader();

    void Bind() const;
    void UnBind() const;

    // Set uniforms
    void SetUniform1i(const std::string &name, int value);
    void SetUniform2i(const std::string &name, int v0, int v1);
    void SetUniform1f(const std::string &name, float value);
    void SetUniform4f(const std::string &name, float v0, float v1, float f2,
                      float f3);
    void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix);

  private:
    unsigned int CreateShader(const std::string &vertexShader,
                              const std::string &fragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string &source);
    ShaderProgramSource ParseShader(const std::string &filepath);

    int GetUniformLocation(const std::string &name);
};
