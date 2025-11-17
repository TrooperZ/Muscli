/**
 * @file Text.h
 * @brief Text class definition
 *
 * Created by Amin Karic, 11/11/25 14:44
 *
 * This class represents a text component that can be added to a Menu. Text is
 * given as a string, and newlines are parsed and width is calculated
 * accordingly.
 */

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include "Component.h"

#ifndef TEXT_H
#define TEXT_H

/**
 * Represents a text object.
 */
class Text : public Component {
   private:
    std::vector<ColoredChar>
        content;  // Stores colored characters. Data is held in one line.
    std::vector<size_t>
        lineBreaks;  // Indexes of the start of each line in content. This is
                     // used for width and height printing.
    std::variant<std::string, std::u32string> rawText;  // Original text.

   public:
    Text();
    Text(uint32_t xCoord, uint32_t yCoord,
         const std::variant<std::string, std::u32string>& textContent,
         uint8_t r, uint8_t g, uint8_t b);
    ~Text();
    std::vector<ColoredChar> getContent() const { return content; }
    void paint(uint32_t color, size_t start = 0, size_t n = 0);

    /**
     * Override for the pixelAt function of the Component class.
     * @param x x coordinate
     * @param y y coordinate
     * @return ColoredChar at (x, y)
     */
    virtual ColoredChar pixelAt(int x, int y) noexcept const {
        if (x < 0 || x >= static_cast<int>(getWidth()) || y < 0 ||
            y >= static_cast<int>(getHeight())) {
            return ColoredChar(' ', 0x000000FF);
        }

        size_t index;
        if (lineBreaks.empty()) {
            index = static_cast<size_t>(x);
        } else {
            index = static_cast<size_t>(x) + lineBreaks[static_cast<size_t>(y)];
        }

        if (index >= content.size()) {
            return ColoredChar('X', 0xFF0000FF);
        }

        return content[index];
    }
};

inline Text::Text() : Component(), content({ColoredChar(' ', 0x000000FF)}) {}

inline Text::Text(uint32_t xCoord, uint32_t yCoord,
                  const std::variant<std::string, std::u32string>& textContent,
                  uint8_t r, uint8_t g, uint8_t b)
    : Component(xCoord, yCoord) {
    rawText = textContent;
    uint32_t rgba = (static_cast<uint32_t>(r) << 24) |
                    (static_cast<uint32_t>(g) << 16) |
                    (static_cast<uint32_t>(b) << 8) | 0xFF;
    std::visit(
        [&](auto const& str) {
            for (auto ch : str) {
                content.push_back(ColoredChar(static_cast<char32_t>(ch), rgba));
            }
        },
        textContent);
    lineBreaks.clear();
    size_t longestLine = 0;
    size_t currentLineLength = 0;
    lineBreaks.push_back(0);

    for (size_t i = 0; i < content.size(); ++i) {
        if (content[i].c == U'\n') {
            if (currentLineLength > longestLine) {
                longestLine = currentLineLength;
            }
            currentLineLength = 0;
            lineBreaks.push_back(i + 1);
        } else {
            ++currentLineLength;
        }
    }
    if (currentLineLength > longestLine) longestLine = currentLineLength;

    setHeight(static_cast<uint32_t>(lineBreaks.size()));
    setWidth(static_cast<uint32_t>(longestLine));
}

inline void Text::paint(uint32_t color, size_t start, size_t n) {
    if (start >= content.size()) return;
    if (n == 0 || start + n > content.size()) {
        n = content.size() - start;
    }
    for (size_t i = start; i < content.size(); ++i) {
        content[i].rgba = color;
    }
}

inline Text::~Text() {
    content.clear();
    lineBreaks.clear();
}

#endif