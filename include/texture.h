#ifndef _TEXTURE
#define _TEXTURE

#define STB_IMAGE_IMPLEMENTATION
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>

class Texture {
public:
    Texture(const std::string filename, const GLint texUnit, const int soilFormat = SOIL_LOAD_AUTO);
    void bind() const;
    void unbind() const;
    GLint activate() const;
    void setFlag(const int glTargetFlag, const int glFlag, const bool bind=false, const bool unbind=false);
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
