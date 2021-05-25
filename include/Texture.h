#ifndef _TEXTURE
#define _TEXTURE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>
#include "Image.h"

class Texture {

public:

    enum FilterFlag{
      MIN_FILTER = GL_TEXTURE_MIN_FILTER,
      MAX_FILTER = GL_TEXTURE_MAG_FILTER
    };
    enum WrapFlag{
      WRAP_S = GL_TEXTURE_WRAP_S,
      WRAP_T = GL_TEXTURE_WRAP_T
    };
    enum FilterValue{
        LINEAR = GL_LINEAR,
        NEAREST = GL_NEAREST,
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
    };
    enum WrapValue{
        REPEAT = GL_REPEAT,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        EDGE_CLAMP = GL_CLAMP_TO_EDGE,
        BORDER_CLAMP = GL_CLAMP_TO_BORDER
    };

    Texture();
    void load(Image & image, const GLint texUnit);
    void load(const std::string filename, const GLint texUnit = GL_TEXTURE0, const int soilFormat = SOIL_LOAD_AUTO);
    void bind() const;
    void unbind() const;
    GLint activate() const;
    void setMinFilter(FilterValue f);
    void setMagFilter(FilterValue f);
    void setWrapS(WrapValue w);
    void setWrapT(WrapValue w);
    const std::string& getFileName() {return filename;}
    void setFileName(const std::string& name) {filename = name;}
    void setFileName(const std::string&& name) {filename = std::move(name);}
    GLint getTexUnit() const;
    operator GLuint() const;

protected:
    static const GLuint textureType;
    GLint texUnit;
    GLuint textureId;
    int forceChannels;
    std::string filename;
    void loadFilters();
    void loadWraps();
    void loadImage(Image * image);
    static GLint glFormat(const int channels);

private:

    FilterValue minFilter = FilterValue::LINEAR;
    FilterValue magFilter = FilterValue::LINEAR;
    WrapValue wrapS = WrapValue::REPEAT;
    WrapValue wrapT = WrapValue::REPEAT;
    WrapValue wrapR = WrapValue::REPEAT;

};

#endif //TEXTURE
