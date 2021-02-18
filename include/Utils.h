#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define DBG(x) std::cout << "\u001b[30;1m [DEBUG]: " << x << "\u001b[0m" << std::endl;
#define MSG(x) std::cout << "\u001b[32m [NOTICE]: " << x << "\u001b[0m" << std::endl;
#define WAR(x) std::cout << "\u001b[33m [WARNING]: " << x << "\u001b[0m" << std::endl;
#define ERR(x) std::cout << "\u001b[31m [ERROR]: " << x << "\u001b[0m" << std::endl;

template <typename T>
std::string vectorToString(std::vector<T>& v){
    std::stringstream output;
    for(unsigned int i=0; i<v.size(); ++i){
        output << v[i];
        output << " ";
    }
    return output.str();
}


class Exception : public std::exception {
    public:
        Exception (const std::string & msg) : msg_(msg) {
            ERR(msg);
        }
        ~Exception() {}

        const char* what() const noexcept {return msg_.c_str();}
    private:
    std::string msg_;
};
