#ifndef _TEXTURESAMPLER
#define _TEXTURESAMPLER

#include "Texture.h"
#include <assimp/scene.h>

class TextureSampler : public Texture {
    public:
        TextureSampler(std::string name, GLint texUnit) : Texture(name, texUnit), uniform(-1) {}

        const std::string& getUniformName() {return uniformName;}
        void setUniformName(const std::string & name) {uniformName = name;}
        void setUniformName(std::string && name) {uniformName = std::move(name);}
        const GLuint getUniform() const {return uniform;}
        void setUniform(GLuint uni) {uniform = uni;}
        aiTextureType getType() const {return type;}
        void setType(aiTextureType type) {this->type = type;}

    private:
        std::string uniformName;
        GLuint uniform;
        aiTextureType type;

};

#endif //_TEXTURESAMPLER
