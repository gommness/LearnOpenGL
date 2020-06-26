#include "../include/vertexShader.h"

VertexShader::VertexShader():Shader() {}
VertexShader::VertexShader(const std::string  & filename): Shader(filename, GL_VERTEX_SHADER) {}
