#pragma once
#include <string>
#include <GL/glew.h>

class Shader {
protected:
    std::string code;
    GLuint type;
    GLuint shader;
    std::string name;

    GLint compile();
    int load(const std::string & filename, const GLuint & type);
public:
    Shader(const std::string  & filename, GLuint type);
    ~Shader();
    operator GLuint() const;
    std::string getName() const;
};
