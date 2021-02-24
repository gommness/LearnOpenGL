#define STB_IMAGE_IMPLEMENTATION
#ifndef _IMAGE
#define _IMAGE

#include <utility>
#include <SOIL/SOIL.h>
#include "Utils.h"

class Image {
    public:
        Image(const std::string & filename, const int soilFormat = SOIL_LOAD_AUTO);
        Image(const Image & other);
        ~Image();

        int getWidth() const {return width;}
        int getHeight() const {return height;}
        int getChannels() const {return channels;}
        const std::string& getFileName() {return filename;}
        unsigned char* getData() {return data;}

    private:
        unsigned char* data = nullptr;
        std::string filename;
        int width;
        int height;
        int channels;
        int soilFormat;
};

#endif //_IMAGE
