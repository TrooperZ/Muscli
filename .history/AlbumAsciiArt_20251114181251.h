// AlbumAsciiArt.h
#ifndef ALBUM_ASCII_ART_H
#define ALBUM_ASCII_ART_H

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "ColoredChar.h"
#include "Component.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"


class AlbumAsciiArt : public Component {
   private:
    std::vector<std::vector<ColoredChar>> content;

   public:
    AlbumAsciiArt();
    AlbumAsciiArt(std::string filepath, uint32_t x, uint32_t y);
    ~AlbumAsciiArt();
    std::vector<std::vector<ColoredChar>> getContent() const { return content; }
    bool loadFromFile(const std::string& filepath);
    void AlbumAsciiArt_Test();

    virtual ColoredChar pixelAt(int x, int y) const {
        if (x < 0 || y < 0 || x >= static_cast<int>(getWidth()) ||
            y >= static_cast<int>(getHeight())) {
            return ColoredChar(' ', 0x000000FF);
        }
        return content[static_cast<size_t>(y)][static_cast<size_t>(x)];
    }
};

inline AlbumAsciiArt::AlbumAsciiArt() : Component(0, 0, 30, 15), content({{}}) {}

inline AlbumAsciiArt::AlbumAsciiArt(std::string filepath, uint32_t x,
                                 uint32_t y)
    : Component(x, y, 30, 15) {
    if (!loadFromFile(filepath)) {
        std::cerr << "Error: Could not load ASCII art from " << filepath
                  << std::endl;
        content = {{ColoredChar('X', 0xFF0000FF)}};
    }
}

inline AlbumAsciiArt::~AlbumAsciiArt() { content.clear(); }

inline bool AlbumAsciiArt::loadFromFile(const std::string& filepath) {
    // We will load the image from a file which we download.
    // We will use stb_image for to resize the image to a 30x15 image.
    // We then take each pixel and convert it to a colored ASCII character.
    // The reasoning behind the 30x15 is that our characters are roughly 2:1
    // height to width ratio, so we want to make it a square.

    int width, height, channels;
    unsigned char* data =
        stbi_load(filepath.data(), &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Error: Failed to load image " << filepath << std::endl;
        return false;
    }

    int outW = 30;
    int outH = 15;
    content.resize(static_cast<size_t>(outH));
    for (auto& row : content) {
        row.resize(static_cast<size_t>(outW));
    }

    std::vector<unsigned char> resized(outW * outH * 4);

    stbir_resize_uint8_srgb(data, width, height, 0, resized.data(), outW, outH,
                            0, STBIR_RGBA);

    // Convert each pixel to a ColoredChar

    for (int y = 0; y < outH; ++y) {
        for (int x = 0; x < outW; ++x) {
            size_t index = static_cast<size_t>(x + y * outW) * 4;
            uint8_t r = resized[index];
            uint8_t g = resized[index + 1];
            uint8_t b = resized[index + 2];
            uint8_t a = resized[index + 3];

            // Compute brightness
            uint8_t brightness =
                static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);

            // Map brightness to ASCII character


            uint32_t color = (static_cast<uint32_t>(r) << 24) |
                             (static_cast<uint32_t>(g) << 16) |
                             (static_cast<uint32_t>(b) << 8) |
                             static_cast<uint32_t>(a);

            content[y][x] = ColoredChar(U'█', color);
        }
    }

    stbi_image_free(data);

    return true;
}

inline void AlbumAsciiArt::AlbumAsciiArt_Test() {
    for (const auto& row : content) {
        for (const auto& pixel : row) {
            std::cout << pixel.ansiColor() << toUTF8(pixel.c) << ColoredChar::ansiReset();
        }
        std::cout << "\n";
    }
}

#endif  // ALBUM_ASCII_ART_H
