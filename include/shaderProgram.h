#pragma once
#include <string>
#include "../include/vertexShader.h"
#include "../include/fragmentShader.h"
#include "../include/texture.h"

class ShaderProgram {
private:
    GLuint shaderProgram;
    std::string name;

    GLint link(const VertexShader & vertex, const FragmentShader & fragment);
public:
    ShaderProgram(const std::string & vertexName, const std::string & fragmentName, const std::string & name="");
    ShaderProgram(const VertexShader & vertex, const FragmentShader & fragment, const std::string & name="");
    void use();
    GLint setUniform(const std::string & uniform, const bool value);
    GLint setUniform(const std::string & uniform, const int value);
    GLint setUniform(const std::string & uniform, const float value);
    GLint setUniform(const std::string & uniform, const Texture & value);
    operator GLuint() const;
};
