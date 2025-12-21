/**
 * @file AlbumAsciiArt.cpp
 * @author Amin Karic
 * @brief Implementation file for AlbumAsciiArt
 * @date 2025-12-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "AlbumAsciiArt.h"
#include <cstdint>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb/stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../../stb/stb_image_resize2.h"

AlbumAsciiArt::AlbumAsciiArt(std::string filepath, int32_t x, int32_t y)
    : Component(x, y, 30, 15) {
    content =
        std::vector<std::vector<ColoredChar>>(15, std::vector<ColoredChar>(30));
    if (!loadFromFile(filepath)) {
        std::cerr << "Error: Could not load ASCII art from " << filepath
                  << std::endl;
        return;
    }
}

AlbumAsciiArt::AlbumAsciiArt(std::string filepath, int32_t x, int32_t y, uint32_t w, uint32_t h){
	// TODO: Modify so that dynamic size works with this,
    content =
        std::vector<std::vector<ColoredChar>>(15, std::vector<ColoredChar>(30));
    if (!loadFromFile(filepath)) {
        std::cerr << "Error: Could not load ASCII art from " << filepath
                  << std::endl;
        return;
    }
};

bool AlbumAsciiArt::loadFromFile(const std::string& filepath) {
    // We will load the image from a file which we download.
    // TODO: Regard the previous todo about changing to direct memory loading
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

            uint32_t color = (static_cast<uint32_t>(r) << 24) |
                             (static_cast<uint32_t>(g) << 16) |
                             (static_cast<uint32_t>(b) << 8) |
                             static_cast<uint32_t>(a);

            content[y][x] = ColoredChar(U'█', color); // Pixel block character
        }
    }

    stbi_image_free(data);

    return true;
}
