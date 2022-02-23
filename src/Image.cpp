#include "Image.h"

Image::Image(const std::string & filename, const int soilFormat){
    this->soilFormat = soilFormat;
    data = SOIL_load_image(filename.c_str(), &width, &height, &channels, soilFormat);
    if(!data) {
        std::string reason = " Reason: " + std::string(SOIL_last_result());
        throw Exception("cannot load image: "+filename + reason);
    }
    this->filename = std::move(filename);
}

Image::Image(const Image & other){
    data = SOIL_load_image_from_memory(other.data, other.height*other.width, &width, &height, &channels, other.soilFormat);
    this->soilFormat = other.soilFormat;
}

Image::~Image(){
    SOIL_free_image_data(data);
}
