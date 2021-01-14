#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <SOIL/SOIL.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"
#include "shaderProgram.h"

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f


void printMatrix(glm::mat4 & mat){
    for(int i = 0; i < 4; i++)
        std::cout << "[0]["<<i<<"]: "<<mat[0][i] <<"[1]["<<i<<"]: "<< mat[1][i] <<"[2]["<<i<<"]: "<< mat[2][i] <<"[3]["<<i<<"]: "<< mat[3][i] << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mainLoop(GLuint shaderProgram, GLuint & VAO, GLFWwindow * window, int nVertices, GLuint texture){
    glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f, 0.0f, 0.0f),
    glm::vec3( 2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f, 2.0f, -2.5f),
    glm::vec3( 1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    glm::mat4 model(1);
    GLfloat radius = 10;
    glm::mat4 view(1);
    glm::mat4 proj(1);
    glm::mat4 id(1);


    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
    proj = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f);

    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex0"), GL_TEXTURE0);

    GLuint modelUniform = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
    GLuint projectionUniform = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(proj));
    GLuint viewUniform = glGetUniformLocation(shaderProgram, "view");

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        GLfloat camX = sin(glfwGetTime())*radius;
        GLfloat camY = cos(glfwGetTime())*radius;
        view = glm::lookAt(glm::vec3(camX, 0.0f, camY), glm::vec3(0,0,0), glm::vec3(0,1,0));
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < sizeof(cubePositions)/sizeof(cubePositions[0]); i++){
            glm::mat4 localTranslation = glm::translate(id, cubePositions[i]);
            localTranslation = glm::rotate(localTranslation, glm::radians(20.0f*i+(GLfloat)glfwGetTime()*20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(localTranslation));
            glDrawArrays(GL_TRIANGLES, 0, nVertices);
        }
        glBindVertexArray(0);

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

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
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
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    GLuint VBO;
    GLuint VAO;

    VertexShader vertex("src/shaders/coordSystems_1.vert");
    FragmentShader fragment("src/shaders/textureSimple.frag");
    ShaderProgram shader(vertex, fragment, "simpleShader");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);


    GLuint texture;
    if((texture = loadTexture("media/questionBlock.jpg", GL_TEXTURE0)) == 0){
        glfwTerminate();
        std::cerr << "failed to load the texture" << std::endl;
        return 1;
    }

    glViewport(0, 0, 800, 600);

    mainLoop(shader, VAO, window, sizeof(vertices)/5, texture);

    glfwTerminate();
    return 0;
}

