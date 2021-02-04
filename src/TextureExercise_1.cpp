#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <SOIL/SOIL.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <assert.h>

#include "Utils.h"
#include "ShaderProgram.h"

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

void mainLoop(GLuint shaderProgram, GLFWwindow * window, std::vector<Rectangle*> & rectangles, GLuint *texture, size_t nTextures){
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(size_t i = 0; i < rectangles.size(); i++){
            for(size_t j=0; j<nTextures; j++){
                glActiveTexture(j);
                glBindTexture(GL_TEXTURE_2D, texture[j]);
                glUniform1i(glGetUniformLocation(rectangles[i]->shader, std::string("tex"+std::to_string(j)).c_str()),j);
            }
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

GLuint loadTexture(const std::string & texName, unsigned int texUnit){
    GLuint texture;
    glGenTextures(1,&texture);
    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
    // texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height;
    unsigned char* data = SOIL_load_image(texName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    if(!data)
        return 0;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(data);
    //glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
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

    VertexShader vertex("src/shaders/TextureExercise.vert");
    FragmentShader fragment("src/shaders/TextureExercise.frag");
    ShaderProgram shader(vertex, fragment, "simpleShader");

    Rectangle t1(vertices_t1, sizeof(vertices_t1), indexes, sizeof(indexes), shader);
    std::vector<Rectangle*> triangles = {&t1};

    GLuint texture[2];
    if((texture[0] = loadTexture("media/container.jpg", GL_TEXTURE0)) == 0){
        glfwTerminate();
        std::cerr << "failed to load the texture" << std::endl;
    }
    if((texture[1] = loadTexture("media/awesomeface.png", GL_TEXTURE1)) == 0){
        glfwTerminate();
        std::cerr << "failed to load the texture" << std::endl;
    }

    // glGenTextures(1,texture);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture[0]);
    // // texture wrapping options
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load and generate the texture
    // int width, height;
    // unsigned char* data = SOIL_load_image("media/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    // if(!data){
    //     glfwTerminate();
    //     std::cerr << "failed to load the texture" << std::endl;
    //     return 1;
    // }
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);
    // SOIL_free_image_data(data);
    // glBindTexture(GL_TEXTURE_2D, 0);




    glViewport(0, 0, 800, 600);

    mainLoop(shader, window, triangles, texture, 2);

    glfwTerminate();
    return 0;
}

