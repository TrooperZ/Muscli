/**
 * @file SeekBar.h
 * @author Amin Karic
 * @brief Implementation of SeekBar component
 * @version 0.1
 * @date 2025-11-28
 *
 * @copyright Copyright (c) 2025
 *
 * This class is the seek bar that shows the current position in the song.
 *
 */

#pragma once

#include <cstdint>

#include "../../ColoredChar/ColoredChar.h"
#include "../Component.h"

/**
 * @brief Represents a seek bar component.
 *
 */
class SeekBar : public Component {
   private:
    uint8_t progress;  // Progress in percentage (0-100)
   public:
    /**
     * @brief Construct a new default Seek Bar object
     *
     */
    SeekBar() = default;
    /**
     * @brief Construct a new Seek Bar object with position and width
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param width width of the seek bar
     */
    SeekBar(uint32_t x, uint32_t y, uint32_t width)
        : Component(x, y, width, 1), progress(0){};

    /**
     * @brief Construct a new Seek Bar object with position, width and progress
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param width width of the seek bar
     * @param prog progress (0-100)
     */
    SeekBar(uint32_t x, uint32_t y, uint32_t width, uint8_t prog)
        : Component(x, y, width, 1), progress(prog){};

    /**
     * @brief Copy constructor
     *
     * @param other SeekBar to copy from
     */
    SeekBar(const SeekBar& other) = default;

    /**
     * @brief Copy assignment operator
     *
     * @param other SeekBar to copy from
     * @return SeekBar&
     */
    SeekBar& operator=(const SeekBar& other) = default;

    /**
     * @brief Move constructor
     *
     * @param other SeekBar to move from
     */
    SeekBar(SeekBar&& other) noexcept = default;

    /**
     * @brief Move assignment operator
     *
     * @param other SeekBar to move from
     * @return SeekBar&
     */
    SeekBar& operator=(SeekBar&& other) noexcept = default;

    /**
     * @brief Set the Progress object
     *
     * @param prog progress (0-100)
     */
    void setProgress(uint8_t prog) { progress = prog > 100 ? 100 : prog; };

    /**
     * @brief Get the Progress object
     *
     * @return uint8_t progress (0-100)
     */
    uint8_t getProgress() const { return progress; };

    /**
     * @brief Return the ColoredChar at given pixel coordinates
     *
     * @param x x coordinate
     * @param y y coordinate
     * @return ColoredChar
     */
    virtual ColoredChar pixelAt(int32_t x, int32_t y) const {
        if (x < 0 || y < 0 || x >= static_cast<int>(getWidth()) ||
            y >= static_cast<int>(getHeight())) {
            return BLANK_CHARACTER;
        }

        // SeekBar has no need to store a character buffer since we can generate
        // the character on the fly

        int32_t filledWidth = (getWidth() * progress) / 100;

        if (x < filledWidth) {
            return ColoredChar(U'─', CCHAR_WHITE);
        } else if (x == filledWidth) {
            return ColoredChar(U'*', CCHAR_WHITE);  // Seek bar dot
        }
        return ColoredChar(U'─', 0x424242FF);  // Dark gray
    };

    /**
     * @brief Destroy the Seek Bar object
     *
     */
    ~SeekBar() = default;
};