/**
 * @file AlbumAsciiArt.h
 * @author Amin Karic
 * @brief AlbumAsciiArt component class definition
 * @version 0.1
 * @date 2025-11-28
 * 
 * @copyright Copyright (c) 2025
 * 
 * This class represents an Album Ascii Art component that can be added to a Menu.
 */
#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "../../ColoredChar/ColoredChar.h"
#include "../Component.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb/stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../../stb/stb_image_resize2.h"


/**
 * @brief Represents an Album Ascii Art component.
 * 
 */
class AlbumAsciiArt : public Component {
   private:
    std::vector<std::vector<ColoredChar>> content;

   public:
   /**
    * @brief Construct a new default Album Ascii Art object
    * 
    */
    AlbumAsciiArt() : Component(0, 0, 30, 15), content({{}}) {};

    /**
     * @brief Construct a new Album Ascii Art object
     * 
     * @param filepath file to load ASCII art from
     * @param x x coordinate
     * @param y y coordinate
     */
    AlbumAsciiArt(std::string filepath, uint32_t x, uint32_t y);

    /**
     * @brief Construct a new Album Ascii Art object
     * 
     * @param filepath file to load ASCII art from
     * @param x x coordinate
     * @param y y coordinate
     * @param w width
     * @param h height
     */
    AlbumAsciiArt(std::string filepath, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

    /**
     * @brief Construct a new Album Ascii Art object by copying another
     * 
     * @param other AlbumAsciiArt to copy from
     */
    AlbumAsciiArt(const AlbumAsciiArt& other) = default;

    /**
     * @brief Copy assignment operator
     * 
     * @param other AlbumAsciiArt to copy from
     * @return AlbumAsciiArt& 
     */
    AlbumAsciiArt& operator=(const AlbumAsciiArt& other) = default;

    /**
     * @brief Construct a new Album Ascii Art object by moving another
     * 
     * @param other AlbumAsciiArt to move from
     */
    AlbumAsciiArt(AlbumAsciiArt&& other) noexcept = default;

    /**
     * @brief Assignment operator by moving another
     * 
     * @param other other AlbumAsciiArt to move from
     * @return AlbumAsciiArt& 
     */
    AlbumAsciiArt& operator=(AlbumAsciiArt&& other) noexcept = default;

    /**
     * @brief Destroy the Album Ascii Art object
     * 
     */
    ~AlbumAsciiArt();

    /**
     * @brief Get the Content object
     * 
     * @return std::vector<std::vector<ColoredChar>> the raw 2d array of ColoredChar representing the ASCII art
     */
    std::vector<std::vector<ColoredChar>> getContent() const { return content; }

    /**
     * @brief Load image and create ASCII art from file
     * 
     * @param filepath file to load image from
     * @return true success
     * @return false failure
     */
    bool loadFromFile(const std::string& filepath);

    // ! TESTING PURPOSES ONLY DELETE ONCE DONE
    void AlbumAsciiArt_Test();

    /**
     * @brief Return the ColoredChar at given pixel coordinates
     * 
     * @param x x coordinate
     * @param y y coordinate
     * @return ColoredChar 
     */
    virtual ColoredChar pixelAt(int32_t x, int32_t y) const {
        if (x < 0 || y < 0 || x >= static_cast<int>(getWidth()) ||
            y >= static_cast<int>(getHeight())) {
            return ColoredChar(' ', 0x000000FF);
        }
        return content[static_cast<size_t>(y)][static_cast<size_t>(x)];
    }
};



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
            std::cout << pixel;
        }
        std::cout << "\n";
    }
}