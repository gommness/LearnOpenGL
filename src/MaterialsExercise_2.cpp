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

#include "Utils.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f

Camera camera;

bool keys[1024];

GLfloat deltaTime = 0;
GLfloat lastFrame = 0;


void processInput(){
    if(keys[GLFW_KEY_W])
       camera.translate(Camera::Movement::FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
       camera.translate(Camera::Movement::BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
       camera.translate(Camera::Movement::LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
       camera.translate(Camera::Movement::RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    static GLfloat lastX = SCREEN_WIDTH/2;
    static GLfloat lastY = SCREEN_HEIGHT/2;
    static bool firstMouse = true;

    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    camera.rotate(xpos-lastX, lastY-ypos, 0, true);
    lastX = xpos;
    lastY = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;

    if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GL_TRUE);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hide the cursor

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
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

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
    };

    glm::vec3 cubePositions[] = {
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

    //glm::vec4 lightPos(-1, -1, 6, 1);
    glm::vec4 lightPos(-1, -1,-1, 1);
    glm::vec3 lightColor(1);
    glm::vec3 diffuseColor(1);
    glm::vec3 ambientColor(1);
    glm::mat4 id(1);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f); 

    Texture texture;
    texture.setMinFilter(Texture::FilterValue::NEAREST);
    texture.setMagFilter(Texture::FilterValue::NEAREST);
    try {
        Image image{"media/container2.jpg"};
        texture.load(image, GL_TEXTURE0);
    } catch (std::runtime_error) {
        glfwTerminate();
        std::cerr << "failed to load the texture" << std::endl;
        return 1;
    }

    VertexShader vertex("src/shaders/DiffuseMap.vert");
    FragmentShader fragment("src/shaders/DiffuseMap.frag");
    ShaderProgram shader(vertex, fragment, "materialShader");

    shader.use();
    GLuint objModelUniform = shader.getUniform("model");
    GLuint objViewUniform = shader.getUniform("view");
    GLuint objProjUniform = shader.getUniform("proj");
    GLuint objLightPosUniform = shader.getUniform("lightPos");
    GLuint objViewPosUniform = shader.getUniform("viewPos");
    GLuint objLightAmbientColor = shader.getUniform("light.ambient");
    GLuint objLightDiffuseColor = shader.getUniform("light.diffuse");

    texture.activate();
    shader.setUniform("material.diffuse", texture);
    shader.setUniform("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setUniform("material.shininess", 32.0f);
    shader.setUniform(objLightAmbientColor, 0.2f, 0.2f, 0.2f);
    shader.setUniform(objLightDiffuseColor, 0.5f, 0.5f, 0.5f);
    shader.setUniform("light.specular", 1.0f, 1.0f, 1.0f);
    // model will be set later
    // view will be set later
    shader.setUniform(objProjUniform , proj);
    //shader.setUniform(objTextureUniform, texture);

    GLuint VBO;
    GLuint VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);



    VertexShader lampVertex("src/shaders/Lamp.vert");
    FragmentShader lampFragment("src/shaders/Lamp.frag");
    ShaderProgram lampShader(lampVertex, lampFragment, "lamp");

    GLuint lampModelUniform = lampShader.getUniform("model");
    GLuint lampViewUniform = lampShader.getUniform("view");
    GLuint lampProjUniform = lampShader.getUniform("proj");
    GLuint lampLightColor = lampShader.getUniform("lightColor");

    lampShader.use();
    // model will be set later
    // view will be set later
    lampShader.setUniform(lampProjUniform , proj);

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // VBO already contains the correct data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 600);


    // DRAWING
    glm::mat4 timedRotation(1);
    //glm::mat4 simpleRotation = glm::rotate(id, glm::radians(1.0f), glm::vec3(1, 1, 0.0f));
    while(!glfwWindowShouldClose(window)){
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        timedRotation = glm::rotate(id, glm::radians((GLfloat)currentFrame*20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        /*
        lightColor.x = sin(currentFrame*2.0f);
        lightColor.y = sin(currentFrame*0.7f);
        lightColor.z = sin(currentFrame*1.3f);
        diffuseColor = lightColor * 0.5f;
        ambientColor = diffuseColor * 0.2f;
        */

        glfwPollEvents();
        processInput();

        // debug::printVector3(camera.position, "camera position: ");
        // debug::printVector3(camera.front, "camera front: ");
        // debug::printMatrix(camera.getViewMatrix(), "viewMatrix");

        glClearColor(0, 0.02f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setUniform(objViewUniform, camera.getViewMatrix());
        shader.setUniform(objViewPosUniform, camera.position.x, camera.position.y, camera.position.z);
        shader.setUniform(objLightPosUniform, lightPos.x, lightPos.y, lightPos.z);
        //shader.setUniform(objLightAmbientColor, ambientColor.x, ambientColor.y, ambientColor.z);
        //shader.setUniform(objLightDiffuseColor, diffuseColor.x, diffuseColor.y, diffuseColor.z);
        // draw cubes
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < sizeof(cubePositions)/sizeof(cubePositions[0]); i++){
            glm::mat4 localTranslation = glm::translate(id, cubePositions[i]);
            localTranslation *= timedRotation;
            localTranslation = glm::rotate(localTranslation, glm::radians(20.0f*i), glm::vec3(0.5f, 1.0f, 0.0f));
            shader.setUniform(objModelUniform, localTranslation);

            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/5);
        }
        // draw lamp
        glBindVertexArray(lightVAO);
        {
            glm::mat4 localTranslation = glm::translate(id, glm::vec3(lightPos));
            localTranslation = glm::scale(localTranslation, glm::vec3(0.2f));
            //localTranslation = timedRotation * localTranslation;
            lampShader.use();
            lampShader.setUniform(lampViewUniform, camera.getViewMatrix());
            lampShader.setUniform(lampModelUniform, localTranslation);
            //lampShader.setUniform(lampModelUniform, glm::mat4(1));
            lampShader.setUniform(lampLightColor, lightColor.x, lightColor.y, lightColor.z);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/5);
        }
        glBindVertexArray(0);

        //lightPos = simpleRotation*lightPos;
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

