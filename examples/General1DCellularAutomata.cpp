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
#include "TemplateShader.h"
#include "Camera.h"

float SCREEN_WIDTH = 800.0f;
float SCREEN_HEIGHT = 800.0f;
int BUFFER_WIDTH = 0;
int BUFFER_HEIGHT = 0;

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

int getFrameBuffer(GLuint & frameBuffer, GLuint & texColorBuffer, Image* image);
int getFrameBuffer(GLuint& frameBuffer, GLuint& texColorBuffer, size_t width, size_t height);

void printHelp(std::ostream& out, const char* programName) {
    out << "usage: " << programName << std::endl;
    out << "-i initialState.png" << std::endl;
    out << "-p programFragmentshader" << std::endl;
    out << "-R rule" << std::endl;
    out << "[-r red]" << std::endl;
    out << "[-g green]" << std::endl;
    out << "[-b blue]" << std::endl;
}

class Buffer {
public:
    GLuint self;
    GLuint texColor;
    Buffer(Image* image) {
        getFrameBuffer(self, texColor, image);
    }
    Buffer(size_t w, size_t h) {
        getFrameBuffer(self, texColor, w, h);
    }
    ~Buffer() {
        glDeleteFramebuffers(1, &self);
    }

    std::string str(){
        return "{frameBuffer: "+std::to_string(self)+", texColor: "+std::to_string(texColor)+"}";
    }
};

void printData(char* data, size_t w, size_t h) {
    printf("\n");
    unsigned int index = 0;
    for (unsigned int r = 0; r < h; r++){
        for(unsigned int c = 0; c < w; c++){
            printf("%x%x%x ", data[index], data[index+1], data[index+2]);
            index += 3;
        }
        printf("\n");
    }
    printf("\n");
}
void printFrameBuffer(GLuint tex, size_t w, size_t h) {
    size_t colorSize = 3;
    char* data = (char*)calloc(w*h, colorSize);
    //glBindTexture(GL_TEXTURE_2D, tex);
    //glGetTexImage(tex, 0, GL_RGB, GL_BYTE, (void*)data);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, tex);
    glReadPixels(0, 0, w, h, GL_RGB, GL_BYTE, data);
    printData(data, w, h);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    free(data);
}

void printTexture(GLuint tex, size_t w, size_t h){
    size_t colorSize = 3;
    char* data = (char*)calloc(w*h, colorSize);
    glBindTexture(GL_TEXTURE_2D, tex);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_BYTE, data);
    printData(data, w, h);
    glBindTexture(GL_TEXTURE_2D, 0);
    free(data);
}

class Automata {
public:
    unsigned long int iteration = 0;
    ShaderProgram* program;
    ShaderProgram& copyProgram;
    Buffer backBuffer;
    Buffer frontBuffer;
    size_t width;
    size_t height;

    Automata (ShaderProgram* prog, ShaderProgram& copyProgram, Image& img) :
        program(prog),
        copyProgram(copyProgram),
        backBuffer(&img),
        frontBuffer(img.getWidth(), img.getHeight()),
        width(img.getWidth()),
        height(img.getHeight())
    {}

    void swap() {
        auto aux = frontBuffer;
        frontBuffer = backBuffer;
        backBuffer = aux;
    }

    void step() {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frontBuffer.self); // drawing to the frame buffer now
        glBindFramebuffer(GL_READ_FRAMEBUFFER, backBuffer.self); // drawing to the frame buffer now
        program->use();

        glBindVertexArray(canvasVAO);
        glBindTexture(GL_TEXTURE_2D, backBuffer.texColor);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, frontBuffer.self);
        glBindTexture(GL_TEXTURE_2D, frontBuffer.texColor);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);
        
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    glfwGetFramebufferSize(window, &BUFFER_WIDTH, &BUFFER_HEIGHT);
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

int getFrameBuffer(GLuint& frameBuffer, GLuint& texColorBuffer, size_t width, size_t height){
    // SETUP FRAME BUFFER
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // frame buffer's color buffer (texture)
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

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
    
    return 0;
}

int getFrameBuffer(GLuint & frameBuffer, GLuint & texColorBuffer, Image* image) {
    // SETUP FRAME BUFFER
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // frame buffer's color buffer (texture)
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData());
    
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

std::string convertToBin(std::string & number) {
    std::string output = "00000000";
    unsigned int n = 0;
    n = std::stoi(number);
    if (n <= 0) {
        return output;
    }
    int i = 0;
    while(n > 0) {
        output[i] = '0' + (n % 2);
        n = n/2;
        i++;
    }
    std::reverse(output.begin(), output.end());
    return output;
}

int main(int argc, char** argv){

    int option;
    std::string initialStateFile;
    std::string programFile("src/shaders/CellularAutomata/general1DAutomata.template.frag");
    std::string automataRule;

    if(argc < 3){
        printHelp(std::cerr, argv[0]);
        return 1;
    }

    while((option = getopt(argc, argv, ":i:p:r:g:b:R:")) != -1) {
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
            case 'R':
                std::string arg = optarg;
                automataRule = convertToBin(arg);
        }
    }
    std::cout << "Automata Rule: " << automataRule << std::endl;

    Image initialState(initialStateFile.c_str());

    //SCREEN_WIDTH = initialState.getWidth();
    //SCREEN_HEIGHT = initialState.getHeight();

    GLFWwindow * window = init();
    if(!window) return 1;

    // canvas geometry and shaders

    std::string rule111 = "return "+std::string{automataRule[0]}+";\n";
    std::string rule110 = "return "+std::string{automataRule[1]}+";\n";
    std::string rule101 = "return "+std::string{automataRule[2]}+";\n";
    std::string rule100 = "return "+std::string{automataRule[3]}+";\n";
    std::string rule011 = "return "+std::string{automataRule[4]}+";\n";
    std::string rule010 = "return "+std::string{automataRule[5]}+";\n";
    std::string rule001 = "return "+std::string{automataRule[6]}+";\n";
    std::string rule000 = "return "+std::string{automataRule[7]}+";\n";
    
    TemplateShader tempShader(programFile.c_str());
    tempShader.addSymbol("rule000", rule000);
    tempShader.addSymbol("rule001", rule001);
    tempShader.addSymbol("rule010", rule010);
    tempShader.addSymbol("rule011", rule011);
    tempShader.addSymbol("rule100", rule100);
    tempShader.addSymbol("rule101", rule101);
    tempShader.addSymbol("rule110", rule110);
    tempShader.addSymbol("rule111", rule111);
    tempShader.preprocess();
    VertexShader copyVertexShader("src/shaders/SimplePass.vert");
    FragmentShader canvasFragmentShader(tempShader);
    FragmentShader copyFragmentShader("src/shaders/CopyShader.frag");

    ShaderProgram canvasShader(copyVertexShader, canvasFragmentShader, "canvasShader");
    
    canvasShader.use();
    GLuint screenTextureUniform = canvasShader.getUniform("screenTexture");
    canvasShader.setUniform(screenTextureUniform, 0);
    canvasShader.setUniform("bufferSize", BUFFER_WIDTH, BUFFER_HEIGHT);
    canvasShader.setUniform("imageSize", initialState.getWidth(), initialState.getHeight());

    copyShader.link(copyVertexShader, copyFragmentShader);

    copyShader.use();
    copyShader.setUniform("screenTexture", 0);
    copyShader.setUniform("bufferSize", BUFFER_WIDTH, BUFFER_HEIGHT);
    copyShader.setUniform("imageSize", initialState.getWidth(), initialState.getHeight());

    FragmentShader testFShader("src/shaders/test.frag");
    ShaderProgram testShader(copyVertexShader, testFShader, "testShader");
    testShader.use();
    testShader.setUniform("screenTexture", 0);
    testShader.setUniform("bufferSize", BUFFER_WIDTH, BUFFER_HEIGHT);
    testShader.setUniform("imageSize", initialState.getWidth(), initialState.getHeight());

    Automata cellularAutomata(&canvasShader, copyShader, initialState);
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


    glViewport(0, 0, BUFFER_WIDTH, BUFFER_HEIGHT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();
        getchar();
        
        cellularAutomata.step();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // drawing to the frame buffer now
        
        copyShader.use();
        glBindVertexArray(canvasVAO);
        glBindTexture(GL_TEXTURE_2D, cellularAutomata.backBuffer.texColor);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        cellularAutomata.swap();
    }

    glfwTerminate();
    return 0;
}

