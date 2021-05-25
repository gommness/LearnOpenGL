#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <assert.h>

#include "Utils.h"
#include "ShaderProgram.h"

#define NDEBUG

class Triangle{
    public:
        GLfloat * vertices;
        size_t nVertices;
        ShaderProgram & shader;
        GLuint VBO;
        GLuint VAO;

    Triangle(GLfloat * vertices, size_t nVertices, ShaderProgram & shader): vertices(vertices), nVertices(nVertices), shader(shader){
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, nVertices, vertices, GL_STATIC_DRAW);
            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(float)));
            glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    void render(){
        static float offset = 0;
        offset +=0.001f;
        shader.use();
        shader.setUniform("offset", offset);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, nVertices);
        glBindVertexArray(0);
    }
};


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mainLoop(GLuint shaderProgram, GLFWwindow * window, std::vector<Triangle*> & triangles){
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(size_t i = 0; i < triangles.size(); i++){
            triangles[i]->render();
        }
        glfwSwapBuffers(window);
    }
}

GLFWwindow * windowInit(){
    GLFWwindow * window;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if(!window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    return window;
}

GLFWwindow* init(){
    GLFWwindow * window;
    window = windowInit();
    if (!window){
        return nullptr;
    }

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    return window;
}

int main(){

    GLFWwindow * window = init();
    if(!window) return 1;

    GLfloat vertices_t1[] = {
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // t1 bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // t1 bottom left
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // t1 top
    };
    assert(sizeof(vertices_t1) % 3 == 0);

    VertexShader vertex("src/shaders/PositionalColor.vert");
    FragmentShader fragment("src/shaders/PositionalColor.frag");
    ShaderProgram shader(vertex, fragment, "simpleShader");

    Triangle t1(vertices_t1, sizeof(vertices_t1), shader);
    std::vector<Triangle*> triangles = {&t1};

    glViewport(0, 0, 800, 600);

    mainLoop(shader, window, triangles);

    glfwTerminate();
    return 0;
}


