/**
 * @file Text.h
 * @author Amin Karic
 * @brief Text component definition.
 * @version 0.1
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025
 *
 * This class represents a text component that can be added to a Menu. Text is
 * given as a string, and newlines are parsed and width is calculated
 * accordingly.
 */

#pragma once

#include <sys/types.h>

#include <cstdint>
#include <string>
#include <vector>

#include "../Component.h"

/**
 * @brief Represents a text object.
 */
class Text : public Component {
   private:
    std::vector<ColoredChar>
        content;  // Stores colored characters. Data is held in one line.
    std::vector<size_t> lineBreaks = {
        0};  // Indexes of the start of each line in content. This is
             // used for width and height printing.

   public:
    /**
     * @brief Construct a new default Text object
     *
     */
    Text() = default;

    /**
     * @brief Copy constructor
     *
     * @param other Text object to copy from
     */
    Text(const Text& other) = default;

    /**
     * @brief Copy assignment operator
     *
     */
    Text& operator=(const Text& other) = default;

    /**
     * @brief Move constructor
     *
     */
    Text(Text&& other) noexcept = default;

    /**
     * @brief Move assignment operator
     *
     */
    Text& operator=(Text&& other) noexcept = default;

    /**
     * @brief Construct a new Text object.
     *
     * @param xCoord x coordinate
     * @param yCoord y coordinate
     * @param textContent text string
     * @param r red color component (0-255)
     * @param g green color component (0-255)
     * @param b blue color component (0-255)
     */
    Text(uint32_t xCoord, uint32_t yCoord, const std::string& textContent,
         uint8_t r, uint8_t g, uint8_t b);
    /**
     * @brief Construct a new Text object with the color as one uint32_t.
     *
     * @param xCoord x coordinate
     * @param yCoord y coordinate
     * @param textContent text string
     * @param rgba 32-bit RGBA color as uint32_t
     */
    Text(uint32_t xCoord, uint32_t yCoord, const std::string& textContent,
         uint32_t rgba)
        : Text(xCoord, yCoord, textContent, (rgba >> 24) & 0xFF,
               (rgba >> 16) & 0xFF, (rgba >> 8) & 0xFF){};

    /**
     * @brief Destroy the Text object
     * 
     */
    ~Text();

    /**
     * @brief Get the raw colored char array.
     *
     * @return std::vector<ColoredChar> array of colored characters
     */
    std::vector<ColoredChar> getContent() const { return content; }

    void paint(uint32_t color, size_t start = 0, size_t n = 0);

    /**
     * Override for the pixelAt function of the Component class.
     * @param x x coordinate
     * @param y y coordinate
     * @return ColoredChar at (x, y)
     */
    virtual ColoredChar pixelAt(int32_t x, int32_t y) const noexcept {
        if (x < 0 || x >= static_cast<int>(getWidth()) || y < 0 ||
            y >= static_cast<int>(getHeight())) {
            return BLANK_CHARACTER;
        }

        // Translate (x, y) to index
        size_t index;
        if (lineBreaks.empty()) {
            index = static_cast<size_t>(x);
        } else {
            index = static_cast<size_t>(x) + lineBreaks[static_cast<size_t>(y)];
        }
        
        return content[index];
    }
};

/**
 * @brief Destroy the Text object.
 *
 */
inline Text::~Text() {
    content.clear();
    lineBreaks.clear();
}
