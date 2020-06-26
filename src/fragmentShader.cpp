#include "../include/fragmentShader.h"

FragmentShader::FragmentShader():Shader() {}
FragmentShader::FragmentShader(const std::string  & filename): Shader(filename, GL_FRAGMENT_SHADER){}
