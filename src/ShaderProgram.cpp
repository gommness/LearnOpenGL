#include "ShaderProgram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string & vertexName, const std::string & fragmentName, const std::string & name){
    VertexShader vertex(vertexName);
    FragmentShader fragment(fragmentName);
    if(name == "")
        this->name = vertexName+"+"+fragmentName;
    else
        this->name = name;
    link(vertex, fragment);
}

ShaderProgram::ShaderProgram(const VertexShader & vertex, const FragmentShader & fragment, const std::string & name){
    if(name == "")
        this->name = vertex.getName()+"+"+fragment.getName();
    else
        this->name = name;
    link(vertex, fragment);
}

GLint ShaderProgram::link(const VertexShader & vertex, const FragmentShader & fragment){
    GLint success;
    GLchar infoLog[512];
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << name << "::LINK_FAILED\n" << infoLog << std::endl;
    }
    return success;
}

ShaderProgram::operator GLuint() const{
    return shaderProgram;
}

void ShaderProgram::use(){
    glUseProgram(shaderProgram);
}

GLuint ShaderProgram::getUniform(const std::string & uniform){
    return glGetUniformLocation(shaderProgram, uniform.c_str());
}

void ShaderProgram::setUniform(const GLuint & uniform, const bool value){
    glUniform1i(uniform, value);
}
void ShaderProgram::setUniform(const GLuint & uniform, const int value){
    glUniform1i(uniform, value);
}
void ShaderProgram::setUniform(const GLuint & uniform, const float value){
    glUniform1f(uniform, value);
}
void ShaderProgram::setUniform(const GLuint & uniform, const Texture & value){
    std::cerr << "AAAAAAA" << std::endl;
    glUniform1i(uniform, value.getTexUnit());
}
void ShaderProgram::setUniform(const GLuint & uniform, const float v1, const float v2){
    glUniform2f(uniform, v1, v2);
}
void ShaderProgram::setUniform(const GLuint & uniform, const float v1, const float v2, const float v3){
    glUniform3f(uniform, v1, v2, v3);
}
void ShaderProgram::setUniform(const GLuint & uniform, const float v1, const float v2, const float v3, const float v4){
    glUniform4f(uniform, v1, v2, v3, v4);
}
void ShaderProgram::setUniform(const GLuint & uniform, const glm::mat4 & value){
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
}

GLint ShaderProgram::setUniform(const std::string & name, const bool value){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1)
        glUniform1i(uniform, value);
    return uniform;
}

GLint ShaderProgram::setUniform(const std::string & name, const int value){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1)
        glUniform1i(uniform, value);
    return uniform;
}

GLint ShaderProgram::setUniform(const std::string & name, const float value){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1)
        glUniform1f(uniform, value);
    return uniform;
}

GLint ShaderProgram::setUniform(const std::string & name, const Texture & texture){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1){
        texture.activate();
        glUniform1i(uniform, texture.getTexUnit() - GL_TEXTURE0);
    }
    return uniform;
}

GLint ShaderProgram::setUniform(TextureSampler & texture){
    GLint uniform = glGetUniformLocation(shaderProgram, texture.getUniformName().c_str());
    if(uniform != -1){
        texture.activate();
        glUniform1i(uniform, texture.getTexUnit() - GL_TEXTURE0);
    }
    return uniform;
}

GLint ShaderProgram::setUniform(const std::string & name, const float v1, const float v2){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1)
        glUniform2f(uniform, v1, v2);
    return uniform;
}

GLint ShaderProgram::setUniform(const std::string & name, const float v1, const float v2, const float v3){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1)
        glUniform3f(uniform, v1, v2, v3);
    return uniform;
}

GLint ShaderProgram::setUniform(const std::string & name, const float v1, const float v2, const float v3, const float v4){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1)
        glUniform4f(uniform, v1, v2, v3, v4);
    return uniform;
}

GLint ShaderProgram::setUniform(const std::string & name, const glm::mat4 & value){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1)
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
    return uniform;
}
