/**
 * @file Text.h
 * @author Amin Karic
 * @brief Text component definition.
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025
 *
 * This class represents a text component that can be added to a Menu. Text is
 * given as a string, and newlines are parsed and width is calculated
 * accordingly. Text is stored as a vector of ColoredChar objects.
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../Component.h"

/**
 * @brief Renderable text component composed of colored characters.
 *
 * @details
 * Text is a non-editable UI component that stores decoded characters internally
 * and renders them via the Component interface. Newlines define explicit
 * line breaks; no automatic word wrapping is performed.
 */
class Text : public Component {
   private:
    std::vector<ColoredChar>
        content;  // Stores colored characters in a flat array.
    std::vector<size_t> lineBreaks = {
        0};  // Indexes of the start of each line in content.

   public:
    Text() = default;

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
    explicit Text(int32_t xCoord, int32_t yCoord,
                  const std::string& textContent, uint8_t r, uint8_t g,
                  uint8_t b);
    /**
     * @brief Construct a new Text object with the color as one uint32_t.
     *
     * @param xCoord x coordinate
     * @param yCoord y coordinate
     * @param textContent text string
     * @param rgba 32-bit RGBA color as uint32_t
     */
    explicit Text(int32_t xCoord, int32_t yCoord,
                  const std::string& textContent, uint32_t rgba)
        : Text(xCoord, yCoord, textContent, (rgba >> 24) & 0xFF,
               (rgba >> 16) & 0xFF, (rgba >> 8) & 0xFF){};

    Text(const Text& other) = default;
    Text& operator=(const Text& other) = default;
    Text(Text&& other) noexcept = default;
    Text& operator=(Text&& other) noexcept = default;

    ~Text() = default;

    /**
     * @brief Paints a portion of the text with a new color.
     *
     * @param color New color as uint32_t RGBA
     * @param start Starting index in content vector
     * @param n Number of characters to paint; if 0, paints to end
     */
    void paint(uint32_t color, size_t start = 0, size_t n = 0);

    /**
     * @brief Override for the pixelAt function of the Component class.
     *
     * @param x x coordinate
     * @param y y coordinate
     * @return ColoredChar at (x, y)
     *
     * @details
     * Coordinates are relative to the Text component's origin. If the
     * coordinate lies outside the text bounds, a blank character is returned.
     */
    virtual ColoredChar pixelAt(int32_t x,
                                int32_t y) const noexcept override final {
        if (x < 0 || x >= static_cast<int>(getWidth()) || y < 0 ||
            y >= static_cast<int>(getHeight())) {
            return BLANK_CHARACTER;
        }

        // Translate (x, y) to index to content vector
        size_t index;
        if (lineBreaks.empty()) {
            index = static_cast<size_t>(x);
        } else {
            index = static_cast<size_t>(x) + lineBreaks[static_cast<size_t>(y)];
        }

        if (index >= content.size()) {
            return BLANK_CHARACTER;
        }

        return content[index];
    }
};
