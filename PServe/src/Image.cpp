//
//  Image.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//

#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_write.h"

inline float clamp(const float& v, const float& a, const float& b) {
    if (v <= a) return a;
    if (v >= b) return b;
    return v;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b) {
    data[0] = r;
    data[1] = g;
    data[2] = b;
}

Color::Color(const glm::vec3& v, int samples) {
     float r = v[0];
     float g = v[1];
     float b = v[2];

     float scale = 1.0f / samples;

     r = sqrtf(scale * r);
     g = sqrtf(scale * g);
     b = sqrtf(scale * b);

     data[0] = static_cast<int>(256 * clamp(r, 0.0f, 0.999f));
     data[1] = static_cast<int>(256 * clamp(g, 0.0f, 0.999f));
     data[2] = static_cast<int>(256 * clamp(b, 0.0f, 0.999f));
}


void saveImage(std::string dir, std::string filename, int width, int height, const void* data){
    stbi_write_bmp((dir + filename + ".bmp").c_str(), width, height, 3, data);
}

void setPixel(int x, int y, int width, int height, glm::vec3 c, float* data, bool flip){
    int index = flip ? (x + (height - 1 - y) * width): (x + y * width);
    index *= 3;
    data[index + 0] = c[0];
    data[index + 1] = c[1];
    data[index + 2] = c[2];
}

void init(int width, int height, float* data, glm::vec3 c){
    for(int i = 0; i < width * height; i++){
        int index = i * 3;
        data[index + 0] = c[0];
        data[index + 1] = c[1];
        data[index + 2] = c[2];
    }
}

inline uint8_t toColor(float a) {
    return (uint8_t)(256 * clamp(a, 0.0, 0.999));
}

void imageToColor(int width, int height, uint8_t* dst, float* src, bool remap)
{
    for (int i = 0; i < width * height * 3; i++) {
        dst[i] = remap ? toColor((src[i] + 1) / 2) : toColor(src[i]);
    }
}
