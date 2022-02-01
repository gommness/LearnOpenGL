#include "TextureSampler.h"
#include <sstream>

std::string toString(TextureSampler& t) {
    std::string output;
    std::ostringstream ss;
    ss << "dir: " << &t /*<< " texture: " << t.getUniformName() << " file: " << t.getFileName()*/;
    return ss.str();
}

std::string toString(TextureSampler* t) {
    return toString(*t);
}
