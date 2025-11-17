/**
 * @file Component.h
 * @brief Component class definition
 *
 * Created by Amin Karic, 11/11/25 14:44
 *
 * This class represents a text component that can be added to a Menu. Text is
 * given as a string, and newlines are parsed and width is calculated
 * accordingly.
 */
#include <cstdint>
#include "ColoredChar.h"
#ifndef COMPONENT_H
#define COMPONENT_H

class Component {
   private:
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;

   public:
    Component();
	Component(uint32_t xCoord, uint32_t yCoord);
    Component(uint32_t xCoord, uint32_t yCoord, uint32_t w, uint32_t h);
    virtual ~Component();
    uint32_t getX() { return x; }
    uint32_t getY() { return y; }
    uint32_t getHeight() { return height; }
    uint32_t getWidth() { return width; }
	void setWidth(uint32_t w) { width = w; }
	void setHeight(uint32_t h) { height = h; }
	void setX(uint32_t xCoord) { x = xCoord; }
	void setY(uint32_t yCoord) { y = yCoord; }
    const uint32_t getX() const { return x; }
    const uint32_t getY() const { return y; }
    const uint32_t getHeight() const { return height; }
    const uint32_t getWidth() const { return width; }
	virtual ColoredChar pixelAt(int x, int y) const;
};

inline Component::Component() : x(0), y(0), width(0), height(0) {}
inline Component::Component(uint32_t xCoord, uint32_t yCoord)
	: x(xCoord), y(yCoord), width(0), height(0) {}
inline Component::Component(uint32_t xCoord, uint32_t yCoord, uint32_t w,
                            uint32_t h)
    : x(xCoord), y(yCoord), width(w), height(h) {}
inline Component::~Component() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
}

inline ColoredChar Component::pixelAt(int x, int y) const {
	return ColoredChar(' ', 0x000000FF);
}

#endif