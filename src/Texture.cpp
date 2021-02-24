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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // bind texture instance with data, and generate mipmaps
    GLint format = Texture::glFormat(image.getChannels());
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.getWidth(), image.getHeight(), 0, format, GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);

    // unbind GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Image& image, const GLint texUnit){
    this->load(image, texUnit);
}

Texture::Texture(const std::string filename, const GLint texUnit, const int soilFormat){
    Image image(filename, soilFormat);
    this->load(image, texUnit);
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

void Texture::setFlag(const int glTargetFlag, const int glFlag, const bool bind, const bool unbind){
    if(bind)
        this->bind();

    glTexParameteri(GL_TEXTURE_2D, glTargetFlag, glFlag);
    
    if(unbind)
        this->unbind();
}

void Texture::setWrap(const Texture::WrapFlag flag, const Texture::WrapValue value, const bool bind, const bool unbind){
    setFlag(flag, bind, bind, unbind);
}
void Texture::setFilter(const Texture::FilterFlag flag, const Texture::FilterValue value, const bool bind, const bool unbind){
    setFlag(flag, bind, bind, unbind);
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
