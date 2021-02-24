#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexShader.h"
#include "FragmentShader.h"
#include "TextureSampler.h"

class ShaderProgram {
private:
    GLuint shaderProgram;
    std::string name;

    GLint link(const VertexShader & vertex, const FragmentShader & fragment);
public:
    ShaderProgram(const std::string & vertexName, const std::string & fragmentName, const std::string & name="");
    ShaderProgram(const VertexShader & vertex, const FragmentShader & fragment, const std::string & name="");
    void use();
    GLuint getUniform(const std::string & uniform);

    void setUniform(const GLuint & uniform, const bool value);
    void setUniform(const GLuint & uniform, const int value);
    void setUniform(const GLuint & uniform, const float value);
    void setUniform(const GLuint & uniform, const Texture & value);
    void setUniform(const GLuint & uniform, const float v1, const float v2);
    void setUniform(const GLuint & uniform, const float v1, const float v2, const float v3);
    void setUniform(const GLuint & uniform, const float v1, const float v2, const float v3, const float v4);
    void setUniform(const GLuint & uniform, const glm::mat4 & value);

    GLint setUniform(const std::string & uniform, const bool value);
    GLint setUniform(const std::string & uniform, const int value);
    GLint setUniform(const std::string & uniform, const float value);
    GLint setUniform(const std::string & uniform, const Texture & value);
    GLint setUniform(TextureSampler & value);
    GLint setUniform(const std::string & uniform, const float v1, const float v2);
    GLint setUniform(const std::string & uniform, const float v1, const float v2, const float v3);
    GLint setUniform(const std::string & uniform, const float v1, const float v2, const float v3, const float v4);
    GLint setUniform(const std::string & uniform, const glm::mat4 & value);
    operator GLuint() const;
};
