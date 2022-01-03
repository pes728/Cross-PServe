//
//  Image.hpp
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//

#ifndef Image_h
#define Image_h

#include "common.h"
#include <string>

class Color {
public:
    Color(uint8_t r, uint8_t g, uint8_t b);

    Color(const glm::vec3& v, int samples);

    uint8_t data[3];
};

inline float clamp(const float& v, const float& a, const float& b);

void saveImage(std::string dir, std::string filename, int width, int height, const void* data);

void setPixel(int x, int y, int width, int height, glm::vec3 c, float* data, bool flip);

void init(int width, int height, float* data, glm::vec3 c);

inline uint8_t toColor(float v);

void imageToColor(int width, int height, uint8_t* dst, float* src, bool remap = false);

#endif /* Image_hpp */
