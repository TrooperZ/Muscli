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
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(c);
}
};

#endif