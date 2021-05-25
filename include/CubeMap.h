#ifndef _CUBEMAP
#define _CUBEMAP

#include "Texture.h"

class CubeMap : public Texture {
    
public:
    enum Face : unsigned int {
        POSITIVE_X = 0,
        NEGATIVE_X,
        POSITIVE_Y,
        NEGATIVE_Y,
        POSITIVE_Z,
        NEGATIVE_Z
    };
    void load(std::vector<Image*> & images, const GLint texUnit);

protected:
    static const GLuint textureType;

private:
    void loadImage(Image* images, unsigned int face);

    FilterValue minFilter = FilterValue::LINEAR;
    FilterValue magFilter = FilterValue::LINEAR;
    WrapValue wrapS = WrapValue::EDGE_CLAMP;
    WrapValue wrapT = WrapValue::EDGE_CLAMP;
    WrapValue wrapR = WrapValue::EDGE_CLAMP;
};

#endif
