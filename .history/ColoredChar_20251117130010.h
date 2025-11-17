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

#ifndef COLORED_CHAR_H
#define COLORED_CHAR_H

//* Basic Color Constants
#define CCHAR_BLACK 0x000000FF
#define CCHAR_WHITE 0xFFFFFFFF
#define CCHAR_RED 0xFF0000FF
#define CCHAR_GREEN 0x00FF00FF
#define CCHAR_BLUE 0x0000FFFF
#define CCHAR_YELLOW 0xFFFF00FF
#define CCHAR_CYAN 0x00FFFFFF
#define CCHAR_MAGENTA 0xFF00FFFF

#define BLANK_CHARACTER ColoredChar(' ', CChar_BLACK)

/**
 * @brief A struct representing a colored character.
 *
 * We store characters as UTF-32 for full Unicode support. The color is stored
 * as a 32-bit RGBA value.
 */

// TODO: Add support for systems which do not support full unicode and full
// colors.
struct ColoredChar {
    char32_t c = ' ';  // Character stored as UTF-32 for full Unicode support.
    uint32_t rgba = CCHAR_WHITE;  // default: white opaque

    ColoredChar() = default;
    ColoredChar(char32_t c, uint32_t color) : c(c), rgba(color) {}

    // Return ANSI 24-bit color escape for foreground (RGB only).
    std::string ansiColor() const {
        uint8_t r = (rgba >> 24) & 0xFF;
        uint8_t g = (rgba >> 16) & 0xFF;
        uint8_t b = (rgba >> 8) & 0xFF;
        std::ostringstream ss;
        ss << "\x1b[38;2;" << int(r) << ';' << int(g) << ';' << int(b) << 'm';
        return ss.str();
    }

    // ANSI reset
    static std::string ansiReset() { return "\x1b[0m"; }
};

// Bless this leetcode question
// https://leetcode.com/problems/utf-8-validation/
inline std::string toUTF8(char32_t c) {
    uint32_t code = static_cast<uint32_t>(c);
    // replacement character for invalid code points
    const uint32_t REPLACEMENT = 0xFFFD;

    if (code > 0x10FFFF) code = REPLACEMENT;
    // surrogate halves are invalid in UTF-32
    if (code >= 0xD800 && code <= 0xDFFF) code = REPLACEMENT;

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
