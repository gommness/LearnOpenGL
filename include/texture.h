#ifndef _TEXTURE
#define _TEXTURE

#define STB_IMAGE_IMPLEMENTATION
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

class Texture {

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
public:
    Texture(const std::string filename, const GLint texUnit, const int soilFormat = SOIL_LOAD_AUTO);
    void bind() const;
    void unbind() const;
    GLint activate() const;
    void setFlag(const int glTargetFlag, const int glFlag, const bool bind=false, const bool unbind=false);
    void setWrap(const WrapFlag flag, const WrapValue value, const bool bind=false, const bool unbind=false);
    void setFilter(const FilterFlag flag, const FilterValue value, const bool bind=false, const bool unbind=false);
    GLint getTexUnit() const;
    operator GLuint() const;
    
private:

    static GLint glFormat(const int channels);

    int width;
    int height;
    int channels;
    int forceChannels;
    GLint texUnit;
    GLuint textureId;

};

class TextureError: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#endif //TEXTURE
