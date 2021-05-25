#include <iostream>
#include <unistd.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <SOIL/SOIL.h>

#include "Model.h"
#include "Utils.h"
#include "ShaderProgram.h"
#include "Camera.h"

float SCREEN_WIDTH = 1024.0f;
float SCREEN_HEIGHT = 1024.0f;

GLfloat canvasVertices[30] = {
    -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,
     1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,

     1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  0.0f,  0.0f,  1.0f
};
GLuint canvasVAO;
GLuint canvasVBO;
glm::vec3 color(1.0f, 1.0f, 1.0f);
ShaderProgram copyShader;

int getFrameBuffer(GLuint & frameBuffer, GLuint & texColorBuffer, Image* image = nullptr);

class Buffer {
public:
    GLuint self;
    GLuint texColor;
    Buffer(Image* image) {
        getFrameBuffer(self, texColor, image);
    }
    ~Buffer() {
        glDeleteFramebuffers(1, &self);
    }

    std::string str(){
        return "{frameBuffer: "+std::to_string(self)+", texColor: "+std::to_string(texColor)+"}";
    }
};

class Automata {
public:
    unsigned long int iteration = 0;
    ShaderProgram* program;
    Buffer backBuffer;
    Buffer frontBuffer;

    Automata (ShaderProgram* prog, Image& img) : program(prog), backBuffer(&img), frontBuffer(nullptr) {
    }

    void swap() {
        auto aux = frontBuffer;
        frontBuffer = backBuffer;
        backBuffer = aux;
    }

    void step() {
        glBindTexture(GL_TEXTURE_2D, frontBuffer.texColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frontBuffer.self); // drawing to the frame buffer now
        glBindFramebuffer(GL_READ_FRAMEBUFFER, backBuffer.self); // drawing to the frame buffer now
        program->use();

        glBindVertexArray(canvasVAO);
        glBindTexture(GL_TEXTURE_2D, backBuffer.texColor);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, frontBuffer.texColor);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //swap();
        //glBindFramebuffer(GL_FRAMEBUFFER, frontBuffer.self); // drawing to the frame buffer now

        iteration++;
    }

};

Automata* globalAutomata;
ShaderProgram* automataProgram;

void error_callback(int error, const char* msg){
    std::cerr << "[ " << std::to_string(error) << "]: " << msg << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_ENTER) {
        globalAutomata->program = automataProgram;
        copyShader.setUniform("colorFactor", color.x, color.y, color.z);
    }
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

int getFrameBuffer(GLuint & frameBuffer, GLuint & texColorBuffer, Image* image) {
    // SETUP FRAME BUFFER
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // frame buffer's color buffer (texture)
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glActiveTexture(GL_TEXTURE0);
    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData());
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // attaching color buffer to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // end of the frame buffer creation process
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
        glDeleteFramebuffers(1, &frameBuffer);
        return -1;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return 0;
}

int strToNumber(char* str, int def) {
    char *end;
    if (str[0] == '\0' || isspace(str[0]))
        return def;
    errno = 0;
    long output = strtol(str, &end, 10);
    if (*end != '\0') {
        return def;
    }
    return output;
}

int main(int argc, char** argv){

    int option;
    std::string initialStateFile;
    std::string programFile;

    while((option = getopt(argc, argv, ":i:p:r:g:b:")) != -1) {
        switch(option) {
            case 'i':
                initialStateFile = optarg;
                break;
            case 'p':
                programFile = optarg;
                break;
            case 'r':
                color.x = ((float)strToNumber(optarg, 255))/255;
                break;
            case 'g':
                color.y = ((float)strToNumber(optarg, 255))/255;
                break;
            case 'b':
                color.z = ((float)strToNumber(optarg, 255))/255;
                break;
        }
    }

    if(argc < 3){
        std::cerr << "not enough arguments." << std::endl;
        std::cerr << "usage: " << argv[0] << " shaderProgram image" << std::endl;
        return 1;
    }

    Image initialState(initialStateFile.c_str());
    SCREEN_WIDTH = initialState.getWidth();
    SCREEN_HEIGHT = initialState.getHeight();

    GLFWwindow * window = init();
    if(!window) return 1;

    // canvas geometry and shaders
    
    VertexShader canvasVertexShader("src/shaders/PostProcessing.vert");
    FragmentShader canvasFragmentShader(programFile.c_str());
    VertexShader copyVertexShader("src/shaders/SimplePass.vert");
    FragmentShader copyFragmentShader("src/shaders/CopyShader.frag");

    ShaderProgram canvasShader(copyVertexShader, canvasFragmentShader, "canvasShader");
    
    canvasShader.use();
    GLuint screenTextureUniform = canvasShader.getUniform("screenTexture");
    canvasShader.setUniform(screenTextureUniform, 0);
    canvasShader.setUniform("scale", SCREEN_WIDTH, SCREEN_HEIGHT);

    copyShader.link(copyVertexShader, copyFragmentShader);

    copyShader.use();
    copyShader.setUniform("screenTexture", 0);
    copyShader.setUniform("scale", SCREEN_WIDTH, SCREEN_HEIGHT);

    Automata cellularAutomata(&copyShader, initialState);
    globalAutomata = &cellularAutomata;
    automataProgram = &canvasShader;


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


    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();
        //getchar();
        
        cellularAutomata.step();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // drawing to the frame buffer now
        //glBindFramebuffer(GL_READ_FRAMEBUFFER, cellularAutomata.backBuffer.self);
        
        copyShader.use();
        glBindVertexArray(canvasVAO);
        glBindTexture(GL_TEXTURE_2D, cellularAutomata.backBuffer.texColor);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //glBindTexture(GL_TEXTURE_2D, cellularAutomata.backBuffer.texColor);
        //glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

        //DBG("ITER:" << cellularAutomata.iteration);
        //usleep(500000);

        glfwSwapBuffers(window);
        cellularAutomata.swap();
        // First pass
    }

    glfwTerminate();
    return 0;
}
