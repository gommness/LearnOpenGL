#pragma once
#include "../include/shader.h"

class VertexShader : public Shader{
public:
    VertexShader();
    VertexShader(const std::string  & filename);
};
