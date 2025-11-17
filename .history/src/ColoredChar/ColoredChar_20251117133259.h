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

#pragma once

#include <codecvt>
#include <cstdint>
#include <locale>
#include <ostream>
#include <sstream>
#include <string>

#include "ColoredChar_Constants.h"
#include "ColoredChar_Helpers.h"

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
