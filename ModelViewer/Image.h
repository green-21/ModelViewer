#pragma once

#include <vector>
// raw pointer를 사용하기 때문에 메모리 해제에 유의할 것.
class Image {
public:
    Image(int width, int height, int channel, std::vector<uint8_t> &raw)
        : width(width), height(height), channel(channel), raw(raw) {}

    Image(const Image &image) {
        width = image.width;
        height = image.height;
        channel = image.channel;
        raw = image.raw;
    }

    int GetWidth() { return width; }
    int GetHeight() { return height; }
    int GetChannel() { return channel; }
    unsigned char *GetData() { return raw.data(); }
    unsigned int GetPitch() const { return width * sizeof(uint8_t) * channel; }

private:
    int width, height, channel;
    std::vector<uint8_t> raw;
};