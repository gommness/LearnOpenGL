#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <assert.h>

#include "../include/utils.h"
#include "../include/shaderProgram.h"

class Rectangle{
    public:
        GLfloat * vertices;
        size_t nVertices;
        GLuint * indexes;
        size_t nIndexes;
        ShaderProgram & shader;
        GLuint texture;
        GLuint VBO;
        GLuint VAO;
        GLuint EBO;

    Rectangle(GLfloat * vertices, size_t nVertices, GLuint * indexes, size_t nIndexes, GLuint texture, ShaderProgram & shader): vertices(vertices), nVertices(nVertices), 
                                                                                                                indexes(indexes), nIndexes(nIndexes), shader(shader),
                                                                                                                texture(texture){
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
        glBindVertexArray(0);
    }

    void render(){
        glBindTexture(GL_TEXTURE_2D, texture);
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

void mainLoop(GLuint shaderProgram, GLFWwindow * window, std::vector<Rectangle*> & rectangles){
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(size_t i = 0; i < rectangles.size(); i++){
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

    GLuint texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("media/container.jpg", &width, &height, &nrChannels, 0);
    if(!data){
        glfwTerminate();
        std::cerr << "failed to load the texture" << std::endl;
        return 1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    VertexShader vertex("src/shaders/textureExercise.vert");
    FragmentShader fragment("src/shaders/textureExercise.frag");
    ShaderProgram shader(vertex, fragment, "simpleShader");

    Rectangle t1(vertices_t1, sizeof(vertices_t1), indexes, sizeof(indexes), texture, shader);
    std::vector<Rectangle*> triangles = {&t1};

    glViewport(0, 0, 800, 600);

    mainLoop(shader, window, triangles);

    glfwTerminate();
    return 0;
}

