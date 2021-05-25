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
#include "CubeMap.h"

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

CubeMap loadCubeMap(const std::string & path, GLuint texUnit){
    CubeMap output;

    std::vector<Image*> images(6);
    Image front(path+"/front.jpg");
    Image back(path+"/back.jpg");
    Image left(path+"/left.jpg");
    Image right(path+"/right.jpg");
    Image top(path+"/top.jpg");
    Image bottom(path+"/bottom.jpg");
    images.push_back(&front);
    images.push_back(&back);
    images.push_back(&left);
    images.push_back(&right);
    images.push_back(&top);
    images.push_back(&bottom);
    output.load(images, texUnit);

    return output;
}

int main(int argc, char** argv){

    if(argc != 2){
        std::cerr << "need 1 argument for loading skybox." << std::endl;
    }


    GLFWwindow * window = init();
    if(!window) return 1;

    CubeMap cubeMap(loadCubeMap(argv[1], GL_TEXTURE0));
    Model model("media/Crate/Crate1.obj");
    VertexShader vShader("src/shaders/ModelMaterial.vert");
    FragmentShader fShader("src/shaders/DepthTestLinear.frag");
    ShaderProgram shader(vShader, fShader, "renderingShader");
    shader.use();

    GLuint objModelUniform = shader.getUniform("model");
    GLuint objViewUniform = shader.getUniform("view");
    GLuint objProjUniform = shader.getUniform("proj");
    GLuint objViewPosUniform = shader.getUniform("viewPos");

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f); 
    glm::mat4 id(1);
    glm::mat4 modelMatrix1(1);
    modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(0.0f, -1.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
    modelMatrix1 *= glm::scale(modelMatrix1, glm::vec3(5.0f, 1.0f, 5.0f));     // Scale it to be like the floor
    glm::mat4 modelMatrix2(1);
    modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(1.5f, 0.0f, 2.5f));
    glm::mat4 modelMatrix3(1);
    modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(-1.5f, 0.0f, 0.0f));

    shader.setUniform(objModelUniform, modelMatrix1);
    shader.setUniform(objProjUniform , projMatrix);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);
    glDepthFunc(GL_LESS);
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

        shader.use();
        shader.setUniform(objViewUniform, camera.getViewMatrix());
        shader.setUniform(objViewPosUniform, camera.position.x, camera.position.y, camera.position.z);

        shader.setUniform(objModelUniform, modelMatrix2);
        model.draw(shader);
        shader.setUniform(objModelUniform, modelMatrix3);
        model.draw(shader);
        shader.setUniform(objModelUniform, modelMatrix1);
        model.draw(shader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
