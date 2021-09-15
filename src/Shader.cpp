#include "Utils.h"
#include "Shader.h"
#include <cstdio>
#include <iostream>

Shader::Shader(const std::string  & filename, GLuint type){
    if(load(filename, type) == -1)
        std::cerr << "error cargando fichero" << std::endl;
    this->compile();
}

Shader::Shader(const TemplateShader& temp, GLuint type) {
    code = temp.getCode();
    this->name = temp.getFileName();
    this->type = type;
    compile();
}

Shader::~Shader(){
    if(shader != 0)
        glDeleteShader(shader);
}

GLint Shader::compile(){
    GLint success;
    GLchar infoLog[512];
    const char *  shader_code;

    shader = glCreateShader(type);
    shader_code = code.c_str();
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        //TODO sustituir esto por una excepcion
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER" << name << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cerr << "shadercode:\n" << code << std::endl;
    }
    return success;
}

int Shader::load(const std::string  & filename, const GLuint & type){
    FILE* fp;

    fp = std::fopen(filename.c_str(), "r");
    if(!fp) return -1;

    std::fseek(fp, 0L, SEEK_END);
    code.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&code[0], 1, code.size(), fp);
    std::fclose(fp);

    this->type = type;
    this->name = filename;
    return code.size();
}

Shader::operator GLuint() const{
    return shader;
}

std::string Shader::getName() const{
    return name;
}
