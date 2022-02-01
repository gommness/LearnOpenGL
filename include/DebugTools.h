#ifndef _DEBUG_TOOLS_H
#define _DEBUG_TOOLS_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace debug{
    void printMatrix(glm::mat4& mat, std::string pre = "");
    void printVector3(glm::vec3& vec, std::string pre = "");

    template <typename T>
    void printVector(std::vector<T>& vec, std::string pre = ""){
        std::cout << pre;
        for (long unsigned int i = 0; i < vec.size(); i++) {
            std::cout << "["<<i<<"]: "<<toString(vec[i]) <<std::endl;
        }
        std::cout << std::endl;
    }
}

#endif
