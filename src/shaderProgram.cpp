#include "../include/shaderProgram.h"
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

GLint ShaderProgram::setUniform(const std::string & name, bool value){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1)
        glUniform1i(uniform, value);
    return uniform;
}

GLint ShaderProgram::setUniform(const std::string & name, int value){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1)
        glUniform1i(uniform, value);
    return uniform;
}

GLint ShaderProgram::setUniform(const std::string & name, float value){
    GLint uniform = glGetUniformLocation(shaderProgram, name.c_str());
    if(uniform != -1)
        glUniform1f(uniform, value);
    return uniform;
}

