/**
 * @file ColoredChar_Helpers.h
 * @author Amin Karic
 * @brief Helper functions for ColoredChar struct
 * @version 0.1
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>

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

/**
 * @brief Decodes a single UTF-8 character from a string starting at index i.
 *
 * @param str input string
 * @param i starting index
 * @param out output char32_t character
 * @return size_t number of bytes consumed
 */
inline size_t decodeUTF8Char(const std::string& str, size_t i, char32_t& out) {
    if (i >= str.size()) {
        throw std::out_of_range("Index out of range in decodeUTF8Char");
    }

    unsigned char byte1 = static_cast<unsigned char>(str[i]);

    if (byte1 <= 0x7F) {
        // 1-byte ASCII
        out = byte1;
        return 1;
    } else if ((byte1 & 0xE0) == 0xC0) {
        // 2-byte sequence
        if (i + 1 >= str.size()) throw std::runtime_error("Invalid UTF-8");
        unsigned char byte2 = static_cast<unsigned char>(str[i + 1]);
        if ((byte2 & 0xC0) != 0x80) throw std::runtime_error("Invalid UTF-8");
        out = ((byte1 & 0x1F) << 6) | (byte2 & 0x3F);
        return 2;
    } else if ((byte1 & 0xF0) == 0xE0) {
        // 3-byte sequence
        if (i + 2 >= str.size()) throw std::runtime_error("Invalid UTF-8");
        unsigned char byte2 = static_cast<unsigned char>(str[i + 1]);
        unsigned char byte3 = static_cast<unsigned char>(str[i + 2]);
        if ((byte2 & 0xC0) != 0x80 || (byte3 & 0xC0) != 0x80)
            throw std::runtime_error("Invalid UTF-8");
        out = ((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);
        return 3;
    } else if ((byte1 & 0xF8) == 0xF0) {
        // 4-byte sequence
        if (i + 3 >= str.size()) throw std::runtime_error("Invalid UTF-8");
        unsigned char byte2 = static_cast<unsigned char>(str[i + 1]);
        unsigned char byte3 = static_cast<unsigned char>(str[i + 2]);
        unsigned char byte4 = static_cast<unsigned char>(str[i + 3]);
        if ((byte2 & 0xC0) != 0x80 || (byte3 & 0xC0) != 0x80 ||
            (byte4 & 0xC0) != 0x80)
            throw std::runtime_error("Invalid UTF-8");
        out = ((byte1 & 0x07) << 18) | ((byte2 & 0x3F) << 12) |
              ((byte3 & 0x3F) << 6) | (byte4 & 0x3F);
        return 4;
    } else {
        throw std::runtime_error("Invalid UTF-8 start byte");
    }
}