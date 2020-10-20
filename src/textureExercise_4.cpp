#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <SOIL/SOIL.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <assert.h>

#include "../include/utils.h"
#include "../include/shaderProgram.h"
#include "../include/texture.h"

class Rectangle{
    public:
        GLfloat * vertices;
        size_t nVertices;
        GLuint * indexes;
        size_t nIndexes;
        ShaderProgram & shader;
        GLuint VBO;
        GLuint VAO;
        GLuint EBO;

    Rectangle(GLfloat * vertices, size_t nVertices, GLuint * indexes, size_t nIndexes, ShaderProgram & shader): vertices(vertices), nVertices(nVertices), 
                                                                                                                indexes(indexes), nIndexes(nIndexes), shader(shader)
                                                                                                                {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, nVertices, vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndexes, indexes, GL_STATIC_DRAW);
            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(float)));
            glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    void render(){
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mainLoop(GLuint shaderProgram, GLFWwindow * window, std::vector<Rectangle*> & rectangles, GLuint texture){
    float x = 0.5;
    float y = 0.5;
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        
        if(glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)
            x-=0.01;
        if(glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
            x+=0.01;
        if(glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
            y-=0.01;
        if(glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
            y+=0.01;
            

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        for(size_t i = 0; i < rectangles.size(); i++){
            rectangles[i]->shader.setUniform("targetCoords", x, y);
            rectangles[i]->render();
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
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    GLuint indexes[] = { // Nota: empezamos desde el 0
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    VertexShader vertex("src/shaders/centralPixel.vert");
    FragmentShader fragment("src/shaders/textureSimple.frag");
    ShaderProgram shader(vertex, fragment, "simpleShader");

    Rectangle t1(vertices_t1, sizeof(vertices_t1), indexes, sizeof(indexes), shader);
    std::vector<Rectangle*> triangles = {&t1};

    Texture texture("media/sample.jpg", GL_TEXTURE0);
    texture.setFlag(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture.setFlag(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    shader.setUniform("tex0", texture);

    glViewport(0, 0, 800, 600);

    mainLoop(shader, window, triangles, texture);

    glfwTerminate();
    return 0;
}

