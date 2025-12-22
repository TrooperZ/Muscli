/**
 * @file Component.h
 * @author Amin Karic
 * @brief Abstract base class for all UI components.
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 * @details
 * Component represents a UI element positioned in a 2D coordinate
 * space. It provides shared layout state (position and size) and defines the
 * rendering contract via a pure virtual pixelAt() function.
 *
 * Component is intended to be subclassed; instantiating it directly is not
 * meaningful.
 */

#pragma once

#include <cstdint>

#include "../ColoredChar/ColoredChar.h"

/**
 * @class Component
 *
 * @brief Base class for renderable UI components.
 *
 * @details
 * Derived classes must implement pixelAt() to describe how the component is
 * rendered at a given coordinate. Coordinates passed to pixelAt() are relative
 * to the component's local origin.
 */
class Component {
   protected:
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;

   public:
    Component() = default;
    Component(int32_t xCoord, int32_t yCoord)
        : x(xCoord), y(yCoord), width(0), height(0) {}
    Component(int32_t xCoord, int32_t yCoord, uint32_t w, uint32_t h)
        : x(xCoord), y(yCoord), width(w), height(h) {}

    Component(const Component& other) = default;
    Component& operator=(Component const& other) = default;
    Component(Component&& other) noexcept = default;
    Component& operator=(Component&& other) noexcept = default;

    virtual ~Component() = default;

    uint32_t getX() const noexcept { return x; }
    uint32_t getY() const noexcept { return y; }
    uint32_t getHeight() const noexcept { return height; }
    uint32_t getWidth() const noexcept { return width; }

    void setWidth(uint32_t w) noexcept { width = w; }
    void setHeight(uint32_t h) noexcept  { height = h; }
    void setX(uint32_t xCoord) noexcept { x = xCoord; }
    void setY(uint32_t yCoord) noexcept { y = yCoord; }

    /**
     * @brief Returns the rendered character at a local coordinate.
     *
     * @param x Local x coordinate relative to the component origin.
     * @param y Local y coordinate relative to the component origin.
     * @return The ColoredChar rendered at (x, y).
     *
     * @note
     * This function must be implemented by all derived components. Implementations
     * should return a BLANK_CHARACTER for coordinates outside the
     * component's visible region.
     */
    virtual ColoredChar pixelAt(int32_t x, int32_t y) const = 0;
};

