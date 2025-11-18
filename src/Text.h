/**
 * @file Text.h
 * @brief Text class definition.
 *
 * Created by Amin Karic, 11/11/25 14:44
 *
 * This class represents a text component that can be added to a Menu. Text is
 * given as a string, and newlines are parsed and width is calculated
 * accordingly.
 */

#pragma once

#include <sys/types.h>

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include "Component/Component.h"

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
    Text() = default;
    Text(const Text& other) = default;
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
    virtual ColoredChar pixelAt(int x, int y) const noexcept {
        // If out of bounds, return blank character
        if (x < 0 || x >= static_cast<int>(getWidth()) || y < 0 ||
            y >= static_cast<int>(getHeight())) {
            return BLANK_CHARACTER;
        }

        // Calculate index in content vector based on line breaks
        size_t index;
        if (lineBreaks.empty()) {
            index = static_cast<size_t>(x);
        } else {
            index = static_cast<size_t>(x) + lineBreaks[static_cast<size_t>(y)];
        }

        // If index is out of bounds, return blank character
        if (index >= content.size()) {
            return BLANK_CHARACTER;
        }

        return content[index];
    }
};

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
inline Text::Text(uint32_t xCoord, uint32_t yCoord,
                  const std::string& textContent, uint8_t r, uint8_t g,
                  uint8_t b)
    : Component(xCoord, yCoord) {
    uint32_t rgba = (static_cast<uint32_t>(r) << 24) |
                    (static_cast<uint32_t>(g) << 16) |
                    (static_cast<uint32_t>(b) << 8) | 0xFF;

    size_t i = 0;
    while (i < textContent.size()) {
        char32_t c;
        size_t len = decodeUTF8Char(textContent, i,
                                    c);  // returns number of bytes consumed
        content.push_back(ColoredChar(c, rgba));
        i += len;
    }
    // Calculate width based on longest line
    size_t longestLine = 0;
    size_t currentLineLength = 0;

    for (size_t i = 0; i < content.size(); ++i) {
        if (content[i].c.bytes[0] == '\n') {
            if (currentLineLength > longestLine) {
                longestLine = currentLineLength;
            }
            currentLineLength = 0;
            lineBreaks.push_back(i + 1);
        } else {
            ++currentLineLength;
        }
    }
    if (currentLineLength > longestLine) {
        longestLine = currentLineLength;
    }

    setHeight(static_cast<uint32_t>(lineBreaks.size()));
    setWidth(static_cast<uint32_t>(longestLine));
}

/**
 * @brief Paints a portion of the text with a new color.
 *
 * @param color New color as uint32_t RGBA
 * @param start Starting index in content vector
 * @param n Number of characters to paint; if 0, paints to end
 */

inline void Text::paint(uint32_t color, size_t start, size_t n) {
    if (start >= content.size()) return;
    if (n == 0 || start + n > content.size()) {
        n = content.size() - start;
    }
    for (size_t i = start; i < content.size(); ++i) {
        content[i].rgba = color;
    }
}

/**
 * @brief Destroy the Text object.
 *
 */
inline Text::~Text() {
    content.clear();
    lineBreaks.clear();
}
