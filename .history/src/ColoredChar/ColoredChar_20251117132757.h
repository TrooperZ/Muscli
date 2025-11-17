/**
 * @file ColoredChar.h
 * @author Amin Karic
 * @brief ColoredChar struct definition
 * @version 0.1
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 * This struct represents a colored character with a 32-bit RGBA color.
 */

#include <codecvt>
#include <cstdint>
#include <locale>
#include <ostream>
#include <sstream>
#include <string>

#include "ColoredChar_Constants.h"
#include "ColoredChar_Helpers.h"

#ifndef COLORED_CHAR_H
#define COLORED_CHAR_H

//* Basic color constants
#define CCHAR_BLACK 0x000000FF
#define CCHAR_WHITE 0xFFFFFFFF
#define CCHAR_RED 0xFF0000FF
#define CCHAR_GREEN 0x00FF00FF
#define CCHAR_BLUE 0x0000FFFF
#define CCHAR_YELLOW 0xFFFF00FF
#define CCHAR_CYAN 0x00FFFFFF
#define CCHAR_MAGENTA 0xFF00FFFF

//* Black character constant
#define BLANK_CHARACTER ColoredChar(' ', CCHAR_BLACK)

//* Unicode characters
#define ANSI_RESET "\x1b[0m"

/**
 * @brief A struct representing a UTF-8 encoded character.
 *
 */
struct Utf8Char {
    uint8_t bytes[4];
    uint8_t length;  // number of bytes actually used (1-4)
};

/**
 * @brief A struct representing a colored character.
 *
 * We store characters as UTF-32 for full Unicode support. The color is stored
 * as a 32-bit RGBA value.
 */

// TODO: Add support for systems which do not support full unicode and full
// colors.
struct ColoredChar {
    Utf8Char c = {' ', 1};        // default: space character
    uint32_t rgba = CCHAR_WHITE;  // default: white opaque

    /**
     * @brief Construct a new Colored Char object
     *
     */
    ColoredChar() = default;

    /**
     * @brief Construct a new Colored Char object with given character and color
     *
     * @param c
     * @param color
     */
    ColoredChar(char32_t c, uint32_t color) : rgba(color) {
        Utf8Char utf8 = {};
        std::string utf8str = toUTF8(c);
        utf8.length = static_cast<uint8_t>(utf8str.size());
        for (size_t i = 0; i < utf8.length; ++i) {
            utf8.bytes[i] = static_cast<uint8_t>(utf8str[i]);
        }
        this->c = utf8;
    }

    /**
     * @brief Converts the ColoredChar to a UTF-8 string with ANSI color codes.
     *
     * @return std::string
     */
    std::string ansiColor() const {
        uint8_t r = (rgba >> 24) & 0xFF;
        uint8_t g = (rgba >> 16) & 0xFF;
        uint8_t b = (rgba >> 8) & 0xFF;
        std::ostringstream ss;
        ss << "\x1b[38;2;" << int(r) << ';' << int(g) << ';' << int(b) << 'm';
        return ss.str();
    }
};

/**
 * @brief Outstream operator overload for ColoredChar.
 *
 * @param os output stream
 * @param cc ColoredChar to output
 * @return std::ostream&
 */
inline std::ostream& operator<<(std::ostream& os, const ColoredChar& cc) {
    os << cc.ansiColor();
    for (size_t i = 0; i < cc.c.length; ++i) {
        os << static_cast<char>(cc.c.bytes[i]);
    }
    os << ANSI_RESET;
    return os;
}

/**
 * @brief UTF-8 encoding function for a single char32_t character.
 *
 * @return std::string UTF-8 encoded string
 */

// https://leetcode.com/problems/utf-8-validation/
inline std::string toUTF8(char32_t c) {
    uint32_t code = static_cast<uint32_t>(c);
    std::string out;

    if (code <= 0x7F) {
        // 1 byte
        out.push_back(static_cast<char>(code));
    } else if (code <= 0x7FF) {
        // 2 bytes
        uint8_t b1 = static_cast<uint8_t>(0xC0 | ((code >> 6) & 0x1F));
        uint8_t b2 = static_cast<uint8_t>(0x80 | (code & 0x3F));
        out.push_back(static_cast<char>(b1));
        out.push_back(static_cast<char>(b2));
    } else if (code <= 0xFFFF) {
        // 3 bytes
        uint8_t b1 = static_cast<uint8_t>(0xE0 | ((code >> 12) & 0x0F));
        uint8_t b2 = static_cast<uint8_t>(0x80 | ((code >> 6) & 0x3F));
        uint8_t b3 = static_cast<uint8_t>(0x80 | (code & 0x3F));
        out.push_back(static_cast<char>(b1));
        out.push_back(static_cast<char>(b2));
        out.push_back(static_cast<char>(b3));
    } else {
        // 4 bytes
        uint8_t b1 = static_cast<uint8_t>(0xF0 | ((code >> 18) & 0x07));
        uint8_t b2 = static_cast<uint8_t>(0x80 | ((code >> 12) & 0x3F));
        uint8_t b3 = static_cast<uint8_t>(0x80 | ((code >> 6) & 0x3F));
        uint8_t b4 = static_cast<uint8_t>(0x80 | (code & 0x3F));
        out.push_back(static_cast<char>(b1));
        out.push_back(static_cast<char>(b2));
        out.push_back(static_cast<char>(b3));
        out.push_back(static_cast<char>(b4));
    }
    return out;
}

#endif
