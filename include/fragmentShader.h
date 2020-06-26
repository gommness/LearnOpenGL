#pragma once
#include "../include/shader.h"

class FragmentShader : public Shader{
public:
    FragmentShader();
    FragmentShader(const std::string  & filename);
};
