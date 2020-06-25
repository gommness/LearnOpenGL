#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <assert.h>

#define NDEBUG
#define STR(X) #X

#define VERTEX_SRC "\
        #version 330 core\n\
        layout (location = 0) in vec3 position;\n\
        void main(){\n\
            gl_Position = vec4(position.x, position.y, position.z, 1.0);\n\
        }"
#define FRAGMENT_SRC "\
        #version 330 core\n\
        out vec4 color;\n\
        void main(){\n\
            color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
        }"
#define FRAGMENT_SRC_2 "\
        #version 330 core\n\
        out vec4 color;\n\
        void main(){\n\
            color = vec4(0f, 1.0f, 1.0f, 1.0f);\n\
        }"

class Triangle{
    public:
        GLfloat * vertices;
        size_t nVertices;
        GLuint shader;
        GLuint VBO;
        GLuint VAO;

    Triangle(GLfloat * vertices, size_t nVertices, GLuint shader): vertices(vertices), nVertices(nVertices), shader(shader){
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, nVertices, vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void render(){
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, nVertices);
        glBindVertexArray(0);
    }
};


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLint compileShader(GLuint & shader, GLuint type, const GLchar** src, const char* shaderName){
    GLint success;
    GLchar infoLog[512];

    shader = glCreateShader(type);
    glShaderSource(shader, 1, src, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER" << shaderName << "COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return success;
}

GLint linkShaders(GLuint & shaderProgram, GLuint & vertexShader, GLuint & fragmentShader){
    GLint success;
    GLchar infoLog[512];
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    return success;
}

void mainLoop(GLuint & shaderProgram, GLFWwindow * window, std::vector<Triangle*> & triangles){
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(size_t i = 0; i < triangles.size(); i++){
            triangles[i]->render();
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


int main(){

    GLfloat vertices_t1[] = {
        -0.5f,  -0.5f, 0.0f, // t1 bottom left
         0.0f,  -0.5f, 0.0f, // t1 bottom right
        -0.25f,  0.0f, 0.0f // t1 top
    };
    GLfloat vertices_t2[] = {
         0.0f,  -0.5f, 0.0f, // t2 bottom left
         0.5f,  -0.5f, 0.0f, // t2 bottom right
         0.25f,  0.0f, 0.0f // t2 top
    };
    GLfloat vertices_t3[] = {
        -0.25f,  0.0f, 0.0f, // t1 top
         0.25f,  0.0f, 0.0f, // t2 top
         0.0f,   0.5f, 0.0f  // t3 top
    };

    assert(sizeof(vertices_t1) % 3 == 0);
    assert(sizeof(vertices_t2) % 3 == 0);
    assert(sizeof(vertices_t3) % 3 == 0);
    GLFWwindow * window;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLuint shaderProgram2;
    const GLchar* vertexShaderSource = VERTEX_SRC;
    const GLchar* fragmentShaderSource = FRAGMENT_SRC;
    const GLchar* fragmentShaderSource2 = FRAGMENT_SRC_2;

    window = windowInit();
    if (!window){
        return 1;
    }

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return 1;
    }

    compileShader(vertexShader, GL_VERTEX_SHADER, &vertexShaderSource, "VERTEX");
    compileShader(fragmentShader, GL_FRAGMENT_SHADER, &fragmentShaderSource, "FRAGMENT");
    linkShaders(shaderProgram, vertexShader, fragmentShader);
    glDeleteShader(fragmentShader);
    compileShader(fragmentShader, GL_FRAGMENT_SHADER, &fragmentShaderSource2, "FRAGMENT");
    linkShaders(shaderProgram2, vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Triangle t1(vertices_t1, sizeof(vertices_t1), shaderProgram);
    Triangle t2(vertices_t2, sizeof(vertices_t2), shaderProgram);
    Triangle t3(vertices_t3, sizeof(vertices_t3), shaderProgram2);
    std::vector<Triangle*> triangles = {&t1, &t2, &t3};


    glViewport(0, 0, 800, 600);

    mainLoop(shaderProgram, window, triangles);

    glfwTerminate();
    return 0;
}
