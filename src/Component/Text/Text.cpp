/**
 * @file Text.cpp
 * @author Amin Karic
 * @brief Text component implementation.
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025
 *
 * @details Implementation file for Text class.
 */

#include "Text.h"
#include <cstdint>

Text::Text(int32_t xCoord, int32_t yCoord, const std::string& textContent,
           uint8_t r, uint8_t g, uint8_t b)
    : Component(xCoord, yCoord) {
    uint32_t rgba = (static_cast<uint32_t>(r) << 24) |
                    (static_cast<uint32_t>(g) << 16) |
                    (static_cast<uint32_t>(b) << 8) | 0xFF;

    // Calculate width based on longest line
    size_t longestLine = 0;
    size_t currentLineLength = 0;

    size_t i = 0;
    while (i < textContent.size()) {
        // Decode UTF-8 character, decoded is an std::pair of the size and the
        // character
        auto decoded = decodeUTF8Char(textContent, i);
        // Ignore newlines in content for cleaner storage; we track line breaks
        // separately
        if (decoded.second != '\n') {
            content.push_back(ColoredChar(decoded.second, rgba));
            ++currentLineLength;
        } else {
            if (currentLineLength > longestLine) {
                longestLine = currentLineLength;
            }
            currentLineLength = 0;
            lineBreaks.push_back(content.size());
        }

        i += decoded.first;
    }
    if (currentLineLength > longestLine) {
        longestLine = currentLineLength;
    }

    setHeight(static_cast<uint32_t>(lineBreaks.size()));
    setWidth(static_cast<uint32_t>(longestLine));

    paintFG(rgba, 0, content.size());
}

void Text::paintFG(uint32_t color, size_t start, size_t n) {
    if (start >= content.size()) {
        return;
    }

    if (n == 0 || start + n > content.size()) {
        n = content.size() - start;
    }

    for (size_t i = start; i < start + n; ++i) {
        content[i].rgba_fg = color;
    }
}

void Text::paintBG(uint32_t color, size_t start, size_t n) {
    // TODO: Implement behaivor
    return;
}