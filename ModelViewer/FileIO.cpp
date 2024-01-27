#include "FileIO.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image FileIO::ReadImage(const std::string filename) {
    int width, height, ch;
    unsigned char *raw = stbi_load(filename.c_str(), &width, &height, &ch, 0);
    size_t size = width * height;

    if (ch == 0) {
        std::exception(raw);
    }

    std::vector<uint8_t> img(size * 4, 255);
    switch (ch) {
    case 1:
        for (size_t i = 0; i < size; i++) {
            for (size_t c = 0; c < 4; c++) {
                img[i * 4 + c] = raw[i];
            }
        }
        break;
    case 2:
    case 3:
    case 4:
        for (size_t i = 0; i < size; i++) {
            for (size_t c = 0; c < ch; c++) {
                img[i * ch + c] = raw[i * ch + c];
            }
        }
        break;
    }

    delete[] raw;
    return Image(width, height, ch, img);
}
