#ifndef _TEXTURESAMPLER
#define _TEXTURESAMPLER

#include "Texture.h"

class TextureSampler : public Texture {
    public:
        TextureSampler(std::string name, GLint texUnit) : Texture(name, texUnit), name(name) {}
        std::string name;
};

#endif //_TEXTURESAMPLER
