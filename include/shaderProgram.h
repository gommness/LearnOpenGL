#pragma once
#include <string>
#include "../include/vertexShader.h"
#include "../include/fragmentShader.h"

class ShaderProgram {
private:
    GLuint shaderProgram;
    VertexShader &vertex;
    FragmentShader &fragment;
    std::string name;

public:
    ShaderProgram(VertexShader & vertex, FragmentShader & fragment, const std::string & name): shaderProgram(0), vertex(vertex),
                                                                                             fragment(fragment), name(name){}
    GLint link();
    operator GLuint() const;
};
