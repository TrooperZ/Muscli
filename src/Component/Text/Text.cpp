/**
 * @file Text.cpp
 * @author Amin Karic
 * @brief Text component implementation.
 * @version 0.1
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025
 *
 * Implementation file for Text class.
 */

#include "Text.h"

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
Text::Text(uint32_t xCoord, uint32_t yCoord, const std::string& textContent,
           uint8_t r, uint8_t g, uint8_t b)
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
void Text::paint(uint32_t color, size_t start, size_t n) {
    if (start >= content.size()) return;
    if (n == 0 || start + n > content.size()) {
        n = content.size() - start;
    }
    for (size_t i = start; i < content.size(); ++i) {
        content[i].rgba = color;
    }
}