#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};


class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indexes;
        std::vector<TextureSampler> textures;

        Mesh(std::vector<Vertex> & vertices, std::vector<GLuint> & indexes, vector<TextureSampler> & textures);
        void draw(ShaderProgram shader);

    private:
        GLuint VAO, VBO, EBO;

        void setUp();
}
