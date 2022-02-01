#include "DebugTools.h"

void debug::printMatrix(glm::mat4& mat, std::string pre){
    for(int i = 0; i < 4; i++)
        std::cout << pre << "[0]["<<i<<"]: "<<mat[0][i] <<"[1]["<<i<<"]: "<< mat[1][i] <<"[2]["<<i<<"]: "<< mat[2][i] <<"[3]["<<i<<"]: "<< mat[3][i] << std::endl;
}

void debug::printVector3(glm::vec3& vec, std::string pre){
    std::cout << pre << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z << std::endl;
}
