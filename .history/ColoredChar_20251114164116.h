#include <codecvt>
#include <cstdint>
#include <locale>
#include <ostream>
#include <sstream>
#include <string>

#ifndef COLORED_CHAR_H
#define COLORED_CHAR_H

// A colored ASCII pixel: character + 32-bit RGBA color.
struct ColoredChar {
    char32_t c = ' ';
    uint32_t rgba = 0xFFFFFFFF;  // default: white opaque

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
        std::string out;
        if (c <= 0x7F) {
            out.push_back(static_cast<char>(c & 0xFF));
        } else if (c <= 0x7FF) {
            out.push_back(static_cast<char>(0xC0 | ((c >> 6) & 0x1F)));
            out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
        } else if (c <= 0xFFFF) {
            out.push_back(static_cast<char>(0xE0 | ((c >> 12) & 0x0F)));
            out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
        } else {
            out.push_back(static_cast<char>(0xF0 | ((c >> 18) & 0x07)));
            out.push_back(static_cast<char>(0x80 | ((c >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
        }
        return out;
    }

#endif