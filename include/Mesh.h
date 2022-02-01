#ifndef _MESH
#define _MESH

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

class Model;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec4 color;
};


class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indexes;
        std::vector<unsigned int> textureIds;
        Model* owner;

        Mesh(Model& owner, std::vector<Vertex> & vertices, std::vector<GLuint> & indexes, std::vector<unsigned int> & textureIds);
        std::string toString();
        void draw(ShaderProgram shader);

    private:
        GLuint VAO, VBO, EBO;

        void setUp();
};

#endif
