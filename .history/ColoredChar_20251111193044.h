#include <string>
#include <sstream>
#include <cstdint>
#include <ostream>
#include <codecvt>
#include <locale>
#include <string>

#ifndef COLORED_CHAR_H
#define COLORED_CHAR_H

// A colored ASCII pixel: character + 32-bit RGBA color.
struct ColoredChar
{
	char32_t c = ' ';
	uint32_t rgba = 0xFFFFFFFF; // default: white opaque

	ColoredChar() = default;
	ColoredChar(char32_t c, uint32_t color) : c(c), rgba(color) {}

	// Return ANSI 24-bit color escape for foreground (RGB only).
	std::string ansiColor() const
	{
		uint8_t r = (rgba >> 24) & 0xFF;
		uint8_t g = (rgba >> 16) & 0xFF;
		uint8_t b = (rgba >> 8) & 0xFF;
		std::ostringstream ss;
		ss << "\x1b[38;2;" << int(r) << ';' << int(g) << ';' << int(b) << 'm';
		return ss.str();
	}

	// ANSI reset
	static std::string ansiReset() { return "\x1b[0m"; }

inline std::string toUTF8(char32_t c) {
    std::string result;
    if (c <= 0x7F) {
        // 1-byte UTF-8
        result.push_back(static_cast<char>(c));
    } else if (c <= 0x7FF) {
        // 2-byte UTF-8
        result.push_back(static_cast<char>(0xC0 | ((c >> 6) & 0x1F)));
        result.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    } else if (c <= 0xFFFF) {
        // 3-byte UTF-8
        result.push_back(static_cast<char>(0xE0 | ((c >> 12) & 0x0F)));
        result.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    } else {
        // 4-byte UTF-8
        result.push_back(static_cast<char>(0xF0 | ((c >> 18) & 0x07)));
        result.push_back(static_cast<char>(0x80 | ((c >> 12) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    }
    return result;
}
};

#endif