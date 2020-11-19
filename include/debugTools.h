#ifndef _DEBUG_TOOLS_H
#define _DEBUG_TOOLS_H

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DBG(X) std::cout << X << std::endl

namespace debug{
    void printMatrix(glm::mat4 mat, std::string pre = "");
    void printVector3(glm::vec3 vec, std::string pre = "");
}

#endif
