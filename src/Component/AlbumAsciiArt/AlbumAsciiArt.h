/**
 * @file AlbumAsciiArt.h
 * @author Amin Karic
 * @brief AlbumAsciiArt component class definition
 * @version 0.1
 * @date 2025-11-28
 *
 * @copyright Copyright (c) 2025
 *
 * This class represents an Album Ascii Art component that can be added to a
 * Menu.
 */
#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include "../../ColoredChar/ColoredChar.h"
#include "../Component.h"

/**
 * @brief Represents an AlbumAsciiArt component.
 *
 * This component renders a fixed-size ASCII representation of album artwork.
 */
class AlbumAsciiArt : public Component {
   private:
    std::vector<std::vector<ColoredChar>>
        content;  // 2D array of ASCII art pixels

   public:
    AlbumAsciiArt()
        : Component(0, 0, 30, 15), content(15, std::vector<ColoredChar>(30)){};

    // TODO: Remove the filepath constructors (or keep for testing until ready)
    // and replace with plain constructors since I'll take in an image straight
    // from memory using my network class to download the image and load it
    // straight into memory.
    /**
     * @brief Construct a new Album Ascii Art object
     *
     * @param filepath file to load ASCII art from
     * @param x x coordinate
     * @param y y coordinate
     */
    explicit AlbumAsciiArt(std::string filepath, int32_t x, int32_t y);

    /**
     * @brief Construct a new Album Ascii Art object
     *
     * @param filepath file to load ASCII art from
     * @param x x coordinate
     * @param y y coordinate
     * @param w width
     * @param h height
     */
    explicit AlbumAsciiArt(std::string filepath, int32_t x, int32_t y,
                           uint32_t w, uint32_t h);

    AlbumAsciiArt(const AlbumAsciiArt& other) = default;
    AlbumAsciiArt& operator=(const AlbumAsciiArt& other) = default;
    AlbumAsciiArt(AlbumAsciiArt&& other) noexcept = default;
    AlbumAsciiArt& operator=(AlbumAsciiArt&& other) noexcept = default;

    ~AlbumAsciiArt() = default;

    /**
     * @brief Get the Content object
     *
     * @return const std::vector<std::vector<ColoredChar>>& the raw 2d array of
     * ColoredChar representing the ASCII art
     */
    const std::vector<std::vector<ColoredChar>>& getContent() const noexcept;

    /**
     * @brief Load image and create ASCII art from file
     *
     * @param filepath file to load image from
     * @return true success
     * @return false failure, original content is not modified
     */
    bool loadFromFile(const std::string& filepath);

    /**
     * @brief Return the ColoredChar at given pixel coordinates
     *
     * @param x x coordinate
     * @param y y coordinate
     * @return ColoredChar colored character at (x, y), BLANK_CHARACTER if out
     * of bounds
     */
    virtual ColoredChar pixelAt(int32_t x,
                                int32_t y) const noexcept override final {
        if (x < 0 || y < 0 || x >= static_cast<int>(getWidth()) ||
            y >= static_cast<int>(getHeight())) {
            return BLANK_CHARACTER;
        }
        return content[static_cast<size_t>(y)][static_cast<size_t>(x)];
    }

    /**
     * @brief Stream insertion operator overload for AlbumAsciiArt. Outputs the
     * art.
     *
     * @param os output stream
     * @param menu AlbumAsciiArt to output
     * @return std::ostream& output stream with AlbumAsciiArt
     */
    friend std::ostream& operator<<(std::ostream& os, const AlbumAsciiArt& a) {
        for (const auto& row : a.content) {
            for (const auto& pixel : row) {
                os << pixel;
            }
            os << '\n';
        }
        return os;
    }
};