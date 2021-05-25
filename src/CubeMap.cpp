#include "CubeMap.h"

const GLuint CubeMap::textureType = GL_TEXTURE_CUBE_MAP;

void CubeMap::load(std::vector<Image*> & images, const GLint texUnit){
    this->texUnit = texUnit;
    if (images.size() > 0 && images[0]) {
        this->filename = images[0]->getFileName();
    }
    // generate texture instance
    glActiveTexture(texUnit);
    bind();

    // set texture default wrapping options
    loadWraps();
    loadFilters();
    for(unsigned int i = Face::POSITIVE_X; i <= Face::NEGATIVE_Z; i+=1){
        loadImage(images[i], i);
    }

    glGenerateMipmap(textureType);

    // unbind textureType
    glBindTexture(textureType, 0);
}

void CubeMap::loadImage(Image* image, unsigned int face){
    // bind texture instance with data, and generate mipmaps
    GLint format = Texture::glFormat(image->getChannels());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, format, image->getWidth(), image->getHeight(), 0, format, GL_UNSIGNED_BYTE, image->getData());
}
