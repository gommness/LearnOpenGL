#include <iostream>

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

    if(argc != 2){
        std::cerr << "ERROR: incorrect number of arguments" << std::endl;
        return 1;
    }

    GLFWwindow * window = init();
    if(!window) return 1;

    Model model(argv[1]);
    VertexShader vShader("src/shaders/AmbientSimple.vert");
    FragmentShader fShader("src/shaders/AmbientSimple.frag");
    ShaderProgram shader(vShader, fShader, "renderingShader");
    shader.use();

    GLuint objProjUniform = shader.getUniform("proj");
    GLuint objViewUniform = shader.getUniform("view");
    GLuint objModelUniform = shader.getUniform("model");
    glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f); 
    glm::mat4 modelMatrix(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
    shader.setUniform(objModelUniform, modelMatrix);
    shader.setUniform(objProjUniform , projMatrix);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    while(!glfwWindowShouldClose(window)){
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        processInput();

        glClearColor(0, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setUniform(objViewUniform, camera.getViewMatrix());


        model.draw(shader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
