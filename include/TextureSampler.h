#ifndef _TEXTURESAMPLER
#define _TEXTURESAMPLER

#include "Texture.h"
#include <assimp/scene.h>
#include <iostream>

class TextureSampler;
std::string toString(TextureSampler& t);
std::string toString(TextureSampler* t);

class TextureSampler : public Texture {
    public:
        const std::string& getUniformName() const {return uniformName;}
        void setUniformName(const std::string & name) {uniformName = name;}
        void setUniformName(std::string && name) {uniformName = std::move(name);}
        const GLuint getUniform() const {return uniform;}
        void setUniform(GLuint uni) {uniform = uni;}
        aiTextureType getType() const {return type;}
        void setType(aiTextureType type) {this->type = type;}

    private:
        std::string uniformName;
        GLuint uniform = -1;
        aiTextureType type;

};

#endif //_TEXTURESAMPLER
