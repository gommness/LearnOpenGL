#include <iostream>
#include <map>
#include <utility>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <SOIL/SOIL.h>

#include "Model.h"
#include "Utils.h"
#include "ShaderProgram.h"
#include "Camera.h"

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f

GLfloat deltaTime = 0;
GLfloat lastFrame = 0;

bool keys[1024];

Camera camera;

void processInput(){
    if(keys[GLFW_KEY_W])
       camera.translate(Camera::Movement::FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
       camera.translate(Camera::Movement::BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
       camera.translate(Camera::Movement::LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
       camera.translate(Camera::Movement::RIGHT, deltaTime);
    if(keys[GLFW_KEY_SPACE])
       camera.translate(Camera::Movement::UP, deltaTime);
    if(keys[GLFW_KEY_LEFT_SHIFT])
       camera.translate(Camera::Movement::DOWN, deltaTime);
}

void error_callback(int error, const char* msg){
    std::cerr << "[ " << std::to_string(error) << "]: " << msg << std::endl;
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

    if(!glfwInit()){
        std::cerr << "Failed to initiate glfw" << std::endl;
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwSetErrorCallback(error_callback);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if(!window){
        std::cerr << "Failed to create GLFW window" << std::endl;
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

int main(int argc, char** argv){

    GLFWwindow * window = init();
    if(!window) return 1;

    GLfloat vertices[] = {
        // positions           // normals           // texture coords
         0.5f,  1.0f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,   // top right
         0.5f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f,   // bottom right
        -0.5f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,   // bottom left
        -0.5f,  1.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f    // top left
    };
    GLuint indexes[] = { // Nota: empezamos desde el 0
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    std::vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3( 3.5f, -1.0f, -0.48f));
    vegetation.push_back(glm::vec3( 6.5f, -1.0f,  0.51f));
    vegetation.push_back(glm::vec3( 5.0f, -1.0f,  0.70f));
    vegetation.push_back(glm::vec3( 4.7f, -1.0f, -2.30f));
    vegetation.push_back(glm::vec3( 5.5f, -1.0f, -0.60f));

    std::vector<glm::vec3> panels;
    panels.push_back(glm::vec3(-0.7f, -1.0f,  2.0f));
    panels.push_back(glm::vec3(-0.5f, -1.0f,  3.0f));
    panels.push_back(glm::vec3( 0.0f, -1.0f,  0.0f));
    panels.push_back(glm::vec3( 0.2f, -1.0f,  1.0f));

    GLuint VBO;
    GLuint VAO;
    GLuint EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    Texture grassTexture;
    grassTexture.setWrapS(Texture::WrapValue::EDGE_CLAMP);
    grassTexture.setWrapT(Texture::WrapValue::EDGE_CLAMP);
    grassTexture.load("media/grass2.png");

    Texture windowTexture;
    windowTexture.load("media/window.png");


    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f); 
    glm::mat4 id(1);
    glm::mat4 modelMatrix1(1);
    glm::mat4 modelMatrix2(1);
    glm::mat4 modelMatrix3(1);
    modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(0.0f, -1.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
    modelMatrix1 *= glm::scale(modelMatrix1, glm::vec3(10.0f, 1.0f, 10.0f));     // Scale it to be like the floor
    modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(1.5f, 0.0f, -7.5f));
    modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(-1.5f, 0.0f, 0.0f));

    Model model("media/Crate/Crate1.obj");
    VertexShader vShader("src/shaders/ModelMaterial.vert");
    FragmentShader fShader("src/shaders/ModelMaterialSimple.frag");
    ShaderProgram shader(vShader, fShader, "renderingShader");
    shader.use();

    GLuint objModelUniform = shader.getUniform("model");
    GLuint objViewUniform = shader.getUniform("view");
    GLuint objProjUniform = shader.getUniform("proj");
    GLuint objViewPosUniform = shader.getUniform("viewPos");
    GLuint objTextureUniform = shader.getUniform("material.texture_diffuse1");

    shader.setUniform(objProjUniform , projMatrix);



    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glViewport(0, 0, 800, 600);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDepthMask(GL_FALSE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)){
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        processInput();


        glClearColor(0, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // drawing the actual models
        shader.use();
        shader.setUniform(objViewUniform, camera.getViewMatrix());
        shader.setUniform(objViewPosUniform, camera.position.x, camera.position.y, camera.position.z);

        // floor
        shader.setUniform(objModelUniform, modelMatrix1);
        model.draw(shader);

        // cubes
        shader.setUniform(objModelUniform, modelMatrix2);
        model.draw(shader);
        //model.draw(shader);

        std::map<float, std::pair<glm::vec3, Texture*>> transparentInstances;
        for(unsigned int i=0; i<vegetation.size(); i++){
            float distance = glm::length(camera.position - vegetation[i]);
            transparentInstances[distance] = std::pair<glm::vec3, Texture*>(vegetation[i], &grassTexture);
        }
        for(unsigned int i=0; i<panels.size(); i++){
            float distance = glm::length(camera.position - panels[i]);
            transparentInstances[distance] = std::pair<glm::vec3, Texture*>(panels[i], &windowTexture);
        }

        // transparent objects
        glBindVertexArray(VAO);
        for(std::map<float, std::pair<glm::vec3, Texture*>>::reverse_iterator it = transparentInstances.rbegin();
                it != transparentInstances.rend(); ++it){
            glm::mat4 matrix(1);
            shader.setUniform(objModelUniform, glm::translate(matrix, it->second.first));
            shader.setUniform(objTextureUniform, *(it->second.second));
            it->second.second->activate();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
        

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
