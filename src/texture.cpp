#include "../include/texture.h"

Texture::Texture(){}

void Texture::load(const std::string filename, const GLint texUnit, const int soilFormat){
    // load image data
    unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, &channels, soilFormat);
    if(!data)
        throw TextureError("cannot load image: "+filename);

    this->texUnit = texUnit;
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
    GLint format = Texture::glFormat(channels);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(data);

    // unbind GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string filename, const GLint texUnit, const int soilFormat){
    this->load(filename, texUnit, soilFormat);
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
