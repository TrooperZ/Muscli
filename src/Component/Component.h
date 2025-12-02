/**
 * @file Component.h
 * @author Amin Karic
 * @brief Component class definition.
 * @version 0.1
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 * This class represents a general component that can be added to a Menu.
 */

#pragma once

#include <cstdint>

#include "../ColoredChar/ColoredChar.h"

/**
 * @brief Represents a component object.
 */
class Component {
   private:
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;

   public:
    /**
     * @brief Construct a new default constructed Component object
     *
     */
    Component() = default;

    /**
     * @brief Construct a new Component object from coordinates.
     *
     * @param xCoord x coordinate
     * @param yCoord y coordinate
     */
    Component(uint32_t xCoord, uint32_t yCoord)
        : x(xCoord), y(yCoord), width(0), height(0) {}

    /**
     * @brief Construct a new Component object from coordinates and dimensions.
     *
     * @param xCoord x coordinate
     * @param yCoord y coordinate
     * @param w width
     * @param h height
     */
    Component(uint32_t xCoord, uint32_t yCoord, uint32_t w, uint32_t h)
        : x(xCoord), y(yCoord), width(w), height(h) {}

    /**
     * @brief Copy constructor
     *
     * @param other the other Component to copy from
     */
    Component(const Component& other) = default;

    /**
     * @brief Copy assignment operator
     *
     * @param other the other Component to copy from
     * @return Component&
     */
    Component& operator=(Component const& other) = default;

    /**
     * @brief Move constructor
     *
     * @param other the other Component to move from
     */
    Component(Component&& other) noexcept = default;

    /**
     * @brief Move assignment operator
     *
     * @param other the other Component to move from
     * @return Component&
     */
    Component& operator=(Component&& other) noexcept = default;

    /**
     * @brief Destroys the Component object
     *
     */
    virtual ~Component();

    /**
     * @brief Gets the x coordinate of the component.
     * ß
     * @return x coordinate
     */
    uint32_t getX() { return x; }
    const uint32_t getX() const { return x; }

    /**
     * @brief Gets the y coordinate of the component.
     *
     * @return y coordinate
     */
    uint32_t getY() { return y; }
    const uint32_t getY() const { return y; }

    /**
     * @brief Gets the height of the component.
     *
     * @return height
     */
    uint32_t getHeight() { return height; }

    const uint32_t getHeight() const { return height; }

    /**
     * @brief Gets the width of the component.
     *
     * @return width
     */
    uint32_t getWidth() { return width; }
    const uint32_t getWidth() const { return width; }

    /**
     * @brief Sets the width of the object.
     *
     * @param w new width
     */
    void setWidth(uint32_t w) { width = w; }

    /**
     * @brief Sets the height of the object.
     *
     * @param h new height
     */
    void setHeight(uint32_t h) { height = h; }

    /**
     * @brief Sets the x coordinate of the object.
     *
     * @param xCoord new x coordinate
     */
    void setX(uint32_t xCoord) { x = xCoord; }

    /**
     * @brief Sets the y coordinate of the object.
     *
     * @param yCoord new y coordinate
     */
    void setY(uint32_t yCoord) { y = yCoord; }

    /**
     * @brief Default pixelAt function.
     *
     * This function is intended to be overridden by derived classes. If it is
     * not, it will return a blank character.
     *
     * @param x x coordinate
     * @param y y coordinate
     * @return ColoredChar at (x, y)
     */
    virtual ColoredChar pixelAt(int32_t x, int32_t y) const { return BLANK_CHARACTER; }
};

/**
 * @brief Destroys the Component object
 */
inline Component::~Component() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
}
