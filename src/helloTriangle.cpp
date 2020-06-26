#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
    return success;
}

void mainLoop(GLuint & shaderProgram, GLuint & VAO, GLFWwindow * window){
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
}

int main(){

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    GLFWwindow * window;
    GLuint VBO;
    GLuint VAO;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    const GLchar* vertexShaderSource = VERTEX_SRC;
    const GLchar* fragmentShaderSource = FRAGMENT_SRC;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if(!window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glGenVertexArrays(1, &VAO);
    // obtenemos un id unico para el buffer de vertices
    glGenBuffers(1, &VBO);

    compileShader(vertexShader, GL_VERTEX_SHADER, &vertexShaderSource, "VERTEX");
    compileShader(fragmentShader, GL_FRAGMENT_SHADER, &fragmentShaderSource, "FRAGMENT");
    linkShaders(shaderProgram, vertexShader, fragmentShader);

    // ya no necesitamos los shaders, asi que los liberamos
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // enlazamos el vertex array object
    glBindVertexArray(VAO);
    // bindeamos el buffer de OpenGL al que hemos creado
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copiamos nuestros vertices al buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // hemos de indicarle a OpenGL que input vamos a determinar (location = 0)
    // la dimension del atributo del vertice (3 porque pasamos vertices en 3D)
    // el tipo de datos de los vertices
    // si OpenGL deberia normalizar las coordenadas o no
    // y el comienzo del vector de datos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
    // NOTA: los atributos de vertices toman los datos del VBO que esta actualmente enlazado al GL_ARRAY_BUFFER
    // a la hora de llamar a glVertexAttribPointer. de esta forma, hemos dicho que el vertice (entrada del shader)
    // de localizacion 0 (en el shader, location = 0) tomara los datos del actual VBO.
    glEnableVertexAttribArray(0);
    // des-enlazamos el VAO
    glBindVertexArray(0);

    glViewport(0, 0, 800, 600);

    mainLoop(shaderProgram, VAO, window);

    glfwTerminate();
    return 0;
}
