/**
 * @file ColoredChar.h
 * @author Amin Karic
 * @brief Glyph + color value type used by the rendering system.
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 * @details
 * ColoredChar represents a single renderable cell consisting of a Unicode
 * code point stored as UTF-32 and a 32-bit RGBA color.
 */

#pragma once

#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>

/**
 * @name ANSI color and character constants
 * @{
 */

/**
 * @brief 32-bit RGBA color constants (0xRRGGBBAA).
 */
inline constexpr uint32_t CCHAR_BLACK = 0x000000FF;
inline constexpr uint32_t CCHAR_WHITE = 0xFFFFFFFF;
inline constexpr uint32_t CCHAR_RED = 0xFF0000FF;
inline constexpr uint32_t CCHAR_GREEN = 0x00FF00FF;
inline constexpr uint32_t CCHAR_BLUE = 0x0000FFFF;
inline constexpr uint32_t CCHAR_YELLOW = 0xFFFF00FF;
inline constexpr uint32_t CCHAR_CYAN = 0x00FFFFFF;
inline constexpr uint32_t CCHAR_MAGENTA = 0xFF00FFFF;

/**
 * @brief ANSI escape sequence that resets all terminal attributes.
 */
inline constexpr const char* ANSI_RESET = "\x1b[0m";

/** @} */

/**
 * @brief Encodes a single UTF-32 code point as UTF-8.
 *
 * @param c Unicode code point.
 * @return UTF-8 encoded string.
 *
 * @note
 * This function is intended for terminal output of individual characters.
 */
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

/**
 * @brief Decodes a single UTF-8 encoded Unicode code point.
 *
 * @details
 * Decodes a UTF-8 sequence starting at byte index @p i in the input
 * string and returns  the decoded UTF-32 code point and the number of bytes
 * consumed (1–4) in an std::pair. 
 *
 * @param str Input UTF-8 encoded string.
 * @param i Starting byte index within @p str.
 *
 * @return std::pair<size_t, char32_t>:
 * - first  = number of bytes consumed
 * - second = decoded Unicode code point (UTF-32)
 *
 * @throws std::out_of_range if @p i is outside the bounds of the string.
 * @throws std::runtime_error if the UTF-8 sequence is malformed.
 */
inline std::pair<size_t, char32_t> decodeUTF8Char(const std::string& str,
                                                  size_t i) {
    if (i >= str.size()) {
        throw std::out_of_range("Index out of range in decodeUTF8Char");
    }

    unsigned char byte1 = static_cast<unsigned char>(str[i]);

    if (byte1 <= 0x7F) {
        // 1-byte ASCII
        return {1, static_cast<char32_t>(byte1)};

    } else if ((byte1 & 0xE0) == 0xC0) {
        // 2-byte sequence
        if (i + 1 >= str.size()) {
            throw std::runtime_error("Invalid UTF-8");
        }
        unsigned char byte2 = static_cast<unsigned char>(str[i + 1]);
        if ((byte2 & 0xC0) != 0x80) {
            throw std::runtime_error("Invalid UTF-8");
        }
        return {2,
                static_cast<char32_t>(((byte1 & 0x1F) << 6) | (byte2 & 0x3F))};

    } else if ((byte1 & 0xF0) == 0xE0) {
        // 3-byte sequence
        if (i + 2 >= str.size()) {
            throw std::runtime_error("Invalid UTF-8");
        }
        unsigned char byte2 = static_cast<unsigned char>(str[i + 1]);
        unsigned char byte3 = static_cast<unsigned char>(str[i + 2]);
        if ((byte2 & 0xC0) != 0x80 || (byte3 & 0xC0) != 0x80) {
            throw std::runtime_error("Invalid UTF-8");
        }
        return {3,
                static_cast<char32_t>(((byte1 & 0x0F) << 12) |
                                      ((byte2 & 0x3F) << 6) | (byte3 & 0x3F))};

    } else if ((byte1 & 0xF8) == 0xF0) {
        // 4-byte sequence
        if (i + 3 >= str.size()) {
            throw std::runtime_error("Invalid UTF-8");
        }
        unsigned char byte2 = static_cast<unsigned char>(str[i + 1]);
        unsigned char byte3 = static_cast<unsigned char>(str[i + 2]);
        unsigned char byte4 = static_cast<unsigned char>(str[i + 3]);
        if ((byte2 & 0xC0) != 0x80 || (byte3 & 0xC0) != 0x80 ||
            (byte4 & 0xC0) != 0x80) {
            throw std::runtime_error("Invalid UTF-8");
        }
        return {4,
                static_cast<char32_t>(((byte1 & 0x1F) << 6) | (byte2 & 0x3F))};

    } else {
        throw std::runtime_error("Invalid UTF-8 start byte");
    }
    
    return {0, 0}; 
}

/**
 * @brief Value type representing a single renderable glyph.
 *
 * @details
 * ColoredChar is a combined class of a UTF-32 character and its 32-bit RGBA
 * color value. It is intended to be stored in contiguous containers (e.g.,
 * std::vector) and performs UTF-8 encoding only during output.
 */

// TODO: Add support for systems which do not support full unicode and full
// colors.
struct ColoredChar {
    char32_t c = ' ';                // default: space character
    uint32_t rgba_fg = CCHAR_WHITE;  // default: white opaque

    // TODO: implement background color support
    uint32_t rgba_bg = CCHAR_BLACK;  // default: black opaque

    ColoredChar() = default;

    /**
     * @brief Constructs a ColoredChar from a Unicode code point.
     *
     * @param c Unicode code point.
     * @param color 32-bit RGBA foreground color (default: white).
     *
     * @details
     * This constructor is intentionally non-explicit to allow implicit
     * conversion from char32_t. The default foreground color is white.
     */
    constexpr ColoredChar(char32_t c, uint32_t color = CCHAR_WHITE)
        : c(c), rgba_fg(color) {}

    ColoredChar(const ColoredChar& other) = default;
    ColoredChar& operator=(ColoredChar const& other) = default;
    ColoredChar(ColoredChar&& other) noexcept = default;
    ColoredChar& operator=(ColoredChar&& other) noexcept = default;

    /**
     * @brief Returns only the ANSI escape sequence for this character's
     * foreground color.
     *
     * @return std::string The ANSI color prefix without the reset code and
     * character.
     */
    std::string getCharFGAnsiColor() const {
        uint8_t r = (rgba_fg >> 24) & 0xFF;
        uint8_t g = (rgba_fg >> 16) & 0xFF;
        uint8_t b = (rgba_fg >> 8) & 0xFF;

        return "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) +
               ";" + std::to_string(b) + "m";
    }

    // TODO: Implement background color ANSI escape sequence
    // std::string getCharBGAnsiColor() const {
    //     uint8_t r = (rgba >> 24) & 0xFF;
    //     uint8_t g = (rgba >> 16) & 0xFF;
    //     uint8_t b = (rgba >> 8) & 0xFF;

    //     return "\x1b[48;2;" + std::to_string(r) + ";" + std::to_string(g) +
    //            ";" + std::to_string(b) + "m";
    // }

    char32_t getRawChar() const { return c; }

    std::string getUTF8Char() const { return toUTF8(c); }
};

/**
 * @brief Outstream operator overload for ColoredChar.
 *
 * @param os output stream
 * @param cc ColoredChar to output
 * @return std::ostream& output stream
 *
 * @details
 * The output includes the ANSI foreground color sequence, the UTF-8 encoded
 * character, and a reset sequence to restore terminal state.
 */
inline std::ostream& operator<<(std::ostream& os, const ColoredChar& cc) {
    os << cc.getCharFGAnsiColor() << cc.getUTF8Char() << ANSI_RESET;
    return os;
}

/**
 * @brief A blank space character rendered with a black foreground color.
 */
inline constexpr ColoredChar BLANK_CHARACTER{' ', CCHAR_BLACK};
