#pragma once
#include <string>
#include <GL/glew.h>

class Shader {
protected:
    std::string code;
    GLuint type;
    GLuint shader;
    std::string name;

public:
    Shader(): code(""), type(0), shader(0), name(""){}
    Shader(const std::string  & filename, GLuint type);
    ~Shader();
    GLint compile();
    int load(const std::string & filename, const GLuint & type);
    void unload();
    operator GLuint() const;
};
