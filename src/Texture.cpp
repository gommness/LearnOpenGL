#include "Texture.h"

Texture::Texture(){}

void Texture::load(Image & image, const GLint texUnit){
    this->texUnit = texUnit;
    this->filename = image.getFileName();
    // generate texture instance
    glGenTextures(1,&textureId);
    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // set texture default wrapping options
    loadWraps();
    loadFilters();

    // bind texture instance with data, and generate mipmaps
    GLint format = Texture::glFormat(image.getChannels());
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.getWidth(), image.getHeight(), 0, format, GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);

    // unbind GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::load(const std::string filename, const GLint texUnit, const int soilFormat){
    Image image(filename, soilFormat);
    load(image, texUnit);

}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLint Texture::activate() const {
    glActiveTexture(texUnit);
    bind();
    return texUnit;
}

void Texture::setMinFilter(FilterValue f){
    minFilter = f;
}

void Texture::setMagFilter(FilterValue f){
    magFilter = f;
}

void Texture::setWrapS(WrapValue w){
    wrapS = w;
}

void Texture::setWrapT(WrapValue w){
    wrapT = w;
}

GLint Texture::getTexUnit() const {
    return this->texUnit;
}

Texture::operator GLuint() const{
    return textureId;
}

GLint Texture::glFormat(const int channels){
    switch (channels){
        case SOIL_LOAD_L:
            return GL_LUMINANCE;
        case SOIL_LOAD_LA:
            return GL_LUMINANCE_ALPHA;
        case SOIL_LOAD_RGB:
            return GL_RGB;
        case SOIL_LOAD_RGBA:
            return GL_RGBA;
        default:
            return 0;
    }
}

void Texture::loadFilters(){
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::loadWraps(){
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}
