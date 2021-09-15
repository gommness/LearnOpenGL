#pragma once
#include "Shader.h"

class FragmentShader : public Shader{
public:
    FragmentShader(const std::string  & filename);
    FragmentShader(const TemplateShader& temp);
};
