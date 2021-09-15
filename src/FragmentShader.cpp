#include "FragmentShader.h"

FragmentShader::FragmentShader(const std::string  & filename): Shader(filename, GL_FRAGMENT_SHADER){}
FragmentShader::FragmentShader(const TemplateShader& temp): Shader(temp, GL_FRAGMENT_SHADER){}
