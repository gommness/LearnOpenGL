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

    if(argc < 2){
        std::cerr << "not enough arguments." << std::endl;
        return 1;
    }

    GLFWwindow * window = init();
    if(!window) return 1;

    // SETUP FRAME BUFFER
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // frame buffer's color buffer (texture)
    GLuint texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attaching color buffer to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    // frame buffer's render buffer (stencil buffer and depth buffer, READ-ONLY)
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // attaching render buffer to the frame buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // end of the frame buffer creation process
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
        glDeleteFramebuffers(1, &framebuffer);
        return 1;
    }
    DBG("Frame buffer correclty set");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    // canvas geometry and shaders
    GLfloat canvasVertices[] = {
        -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,

         1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  0.0f,  0.0f,  1.0f
    };
    GLuint canvasVAO;
    GLuint canvasVBO;
    glGenVertexArrays(1, &canvasVAO);
    glGenBuffers(1, &canvasVBO);

    glBindVertexArray(canvasVAO);
        glBindBuffer(GL_ARRAY_BUFFER, canvasVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(canvasVertices), canvasVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    VertexShader canvasVertexShader("src/shaders/PostProcessing.vert");
    FragmentShader canvasFragmentShader(argv[1]);
    ShaderProgram canvasShader(canvasVertexShader, canvasFragmentShader, "canvasShader");
    canvasShader.use();
    GLuint screenTextureUniform = canvasShader.getUniform("screenTexture");
    canvasShader.setUniform(screenTextureUniform, 0);




    // scene gemoetry and shaders
    Model model("media/Crate/Crate1.obj");
    VertexShader vShader("src/shaders/ModelMaterial.vert");
    FragmentShader fShader("src/shaders/ModelMaterialSimple.frag");
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

        // First pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // drawing to the frame buffer now
        glClearColor(0, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        shader.use();
        shader.setUniform(objViewUniform, camera.getViewMatrix());
        shader.setUniform(objViewPosUniform, camera.position.x, camera.position.y, camera.position.z);

        shader.setUniform(objModelUniform, modelMatrix2);
        model.draw(shader);
        shader.setUniform(objModelUniform, modelMatrix3);
        model.draw(shader);
        shader.setUniform(objModelUniform, modelMatrix1);
        model.draw(shader);

        // Second pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        canvasShader.use();
        glBindVertexArray(canvasVAO);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteFramebuffers(1, &framebuffer);
    glfwTerminate();
    return 0;
}
