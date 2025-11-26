/**
 * @file Utf8Char.h
 * @author Amin Karic
 * @brief Utf8Char struct definition
 * @version 0.1
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 * This struct represents a Utf-8 encoded character.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "../ColoredChar_Helpers.h"

/**
 * @brief A struct representing a UTF-8 encoded character.
 *
 */

struct Utf8Char {
    uint8_t* bytes;  // pointer to byte array, we only allocate what we need
    uint8_t length;  // number of bytes actually used (1-4)

    /**
     * @brief Construct a new Utf 8 Char object
     *
     */
    Utf8Char() {
        bytes = new uint8_t[1]{' '};
        length = 1;
    }

    /**
     * @brief Construct a new Utf 8 Char object from a char32_t
     *
     * @param c character to convert in UTF-32
     */
    Utf8Char(char32_t c) {
        std::string utf8str = toUTF8(c);
        length = static_cast<uint8_t>(utf8str.size());
        bytes = new uint8_t[length];
        for (size_t i = 0; i < length; ++i) {
            bytes[i] = static_cast<uint8_t>(utf8str[i]);
        }
    }

    /**
     * @brief Construct a new Utf 8 Char object
     *
     * @param other the other Utf8Char to copy from
     */
    Utf8Char(const Utf8Char& other) {
        length = other.length;
        bytes = new uint8_t[length];
        for (size_t i = 0; i < length; ++i) {
            bytes[i] = other.bytes[i];
        }
    }

    /**
     * @brief Copy assignment operator
     *
     * @param other the other Utf8Char to copy from
     * @return Utf8Char&
     */
    Utf8Char& operator=(const Utf8Char& other) {
        if (this != &other) {
            delete[] bytes;
            length = other.length;
            bytes = new uint8_t[length];
            for (size_t i = 0; i < length; ++i) {
                bytes[i] = other.bytes[i];
            }
        }
        return *this;
    }

    /**
     * @brief Move constructor
     *
     * @param other the other Utf8Char to move from
     */
    Utf8Char(Utf8Char&& other) noexcept {
        bytes = other.bytes;
        length = other.length;
        other.bytes = nullptr;
        other.length = 0;
    }

    /**
     * @brief Move assignment operator
     *
     * @param other the other Utf8Char to move from
     * @return Utf8Char&
     */
    Utf8Char& operator=(Utf8Char&& other) noexcept {
        if (this != &other) {
            delete[] bytes;
            bytes = other.bytes;
            length = other.length;
            other.bytes = nullptr;
            other.length = 0;
        }
        return *this;
    }

    /**
     * @brief Destroy the Utf 8 Char object
     *
     */
    ~Utf8Char() {
        delete[] bytes;
        bytes = nullptr;
        length = 0;
    }
};