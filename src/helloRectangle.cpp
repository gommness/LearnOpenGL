#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define OK 1
#define ERR 0
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



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLint compileShader(GLuint & shader, GLuint type, const GLchar** src, const char* shaderName){
    GLint success;
    GLchar infoLog[512];

    // creamos el objeto shader
    shader = glCreateShader(type);
    // cargamos el codigo del shader
    glShaderSource(shader, 1, src, NULL);
    // compilamos el shader
    glCompileShader(shader);
    // comprobamos si la compilacion del shader ha sido correcta o no
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
}

void mainLoop(GLuint & shaderProgram, GLuint & VAO, GLFWwindow * window){
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
}

GLFWwindow * glfwSetUp(){
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

int glewSetUp(){
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return ERR;
    }
    return OK;
}

int main(){

    GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f, // Top right
         0.5f, -0.5f, 0.0f, // Bottom right
        -0.5f, -0.5f, 0.0f, // Bottom left
        -0.5f,  0.5f, 0.0f  // Top left
    };
    GLuint indexes[] = { // Nota: empezamos desde el 0
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    GLFWwindow * window;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLint success;
    GLchar infoLog[512];
    const GLchar* vertexShaderSource = VERTEX_SRC;
    const GLchar* fragmentShaderSource = FRAGMENT_SRC;

    if((window = glfwSetUp()) == nullptr)
        return 1;

    if(glewSetUp() == ERR){
        glfwTerminate();
        return 1;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    compileShader(vertexShader, GL_VERTEX_SHADER, &vertexShaderSource, "VERTEX");
    compileShader(fragmentShader, GL_FRAGMENT_SHADER, &fragmentShaderSource, "FRAGMENT");
    linkShaders(shaderProgram, vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glViewport(0, 0, 800, 600);

    mainLoop(shaderProgram, VAO, window);

    glfwTerminate();
    return 0;
}
