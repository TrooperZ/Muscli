/**
 * @file SeekBar.h
 * @author Amin Karic
 * @brief SeekBar component definition.
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
 * @brief Represents a horizontal seek bar indicating playback progress.
 */
class SeekBar : public Component {
   private:
    uint8_t progress;  // Progress in percentage [0, 100]
   public:
    SeekBar() = default;
    /**
     * @brief Construct a new SeekBar object with position and width
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param width width of the seek bar
     */
    explicit SeekBar(int32_t x, int32_t y, uint32_t width)
        : Component(x, y, width, 1), progress(0){};

    /**
     * @brief Construct a new Seek Bar object with position, width and progress
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param width width of the seek bar
     * @param prog progress (0-100)
     */
    explicit SeekBar(int32_t x, int32_t y, uint32_t width, uint8_t prog)
        : Component(x, y, width, 1), progress(prog > 100 ? 100 : prog){};

    SeekBar(const SeekBar& other) = default;
    SeekBar& operator=(const SeekBar& other) = default;
    SeekBar(SeekBar&& other) noexcept = default;
    SeekBar& operator=(SeekBar&& other) noexcept = default;

    ~SeekBar() = default;

    void setProgress(uint8_t prog) { progress = prog > 100 ? 100 : prog; };

    uint8_t getProgress() const { return progress; };

    /**
     * @brief Return the ColoredChar at given pixel coordinates
     *
     * @param x x coordinate
     * @param y y coordinate
     * @return ColoredChar
     */
    virtual ColoredChar pixelAt(int32_t x, int32_t y) const noexcept override final {
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
        return ColoredChar(U'─', 0x424242FF);  // Dark gray color
    };
};