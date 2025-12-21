// ! Button.h will be implemented later with the arrow key model

// /**
//  * @file Button.h
//  * @author Amin Karic
//  * @brief Button UI component definition.
//  * @date 2025-12-20
//  *
//  * @copyright Copyright (c) 2025
//  * 
//  * The Button component represents a UI element that displays text
//  * and visually reflects a pressed or unpressed state. It does not own any
//  * behavior or input handling logic; it is purely a renderable component.
//  */

// #pragma once

// #include <cstdint>
// #include <string>
// #include "../Component.h"
// #include "../../ColoredChar/ColoredChar.h"
// #include "../Text/Text.h"

// class Button : public Component {
// private:
//     Text text; // Content on top of button, text will 
//     bool pressed;

// public:
//     Button();
//     Button(uint32_t xCoord, uint32_t yCoord);
//     Button(uint32_t xCoord, uint32_t yCoord, uint32_t w, uint32_t h, const std::string &t = "", uint32_t fg = 0xFFFFFFFF, uint32_t bg = 0x000000FF);
//     virtual ~Button();
//     void setForeground(uint32_t c) { fgColor = c; }
//     void setBackground(uint32_t c) { bgColor = c; }
//     void setPressed(bool p) { pressed = p; }
//     bool isPressed() const { return pressed; }
//     virtual ColoredChar pixelAt(int32_t x, int32_t y) const;
// };

// // inline Button::Button() : Component(), text(""), fgColor(0xFFFFFFFF), bgColor(0x000000FF), pressed(false) {}
// // inline Button::Button(uint32_t xCoord, uint32_t yCoord) : Component(xCoord, yCoord), text(""), fgColor(0xFFFFFFFF), bgColor(0x000000FF), pressed(false) {}
// // inline Button::Button(uint32_t xCoord, uint32_t yCoord, uint32_t w, uint32_t h, const std::string &t, uint32_t fg, uint32_t bg)
// //     : Component(xCoord, yCoord, w, h), text(t), fgColor(fg), bgColor(bg), pressed(false) {}
// // inline Button::~Button() {
// //     text.clear();
// //     fgColor = 0;
// //     bgColor = 0;
// //     pressed = false;
// // }

// // inline ColoredChar Button::pixelAt(int32_t x, int32_t y) const {
// //     int lx = x - static_cast<int>(getX());
// //     int ly = y - static_cast<int>(getY());
// //     if (lx < 0 || ly < 0 || static_cast<uint32_t>(lx) >= getWidth() || static_cast<uint32_t>(ly) >= getHeight())
// //         return ColoredChar(' ', bgColor);
// //     uint32_t w = getWidth();
// //     uint32_t h = getHeight();
// //     if (h == 0 || w == 0)
// //         return ColoredChar(' ', bgColor);
// //     int row = static_cast<int>(h) / 2;
// //     if (ly != row)
// //         return ColoredChar(' ', bgColor);
// //     size_t len = text.size();
// //     if (len == 0)
// //         return ColoredChar(' ', bgColor);
// //     int start = 0;
// //     if (static_cast<uint32_t>(len) < w)
// //         start = static_cast<int>((w - static_cast<uint32_t>(len)) / 2);
// //     int pos = lx - start;
// //     if (pos < 0 || static_cast<size_t>(pos) >= len)
// //         return ColoredChar(' ', bgColor);
// //     char c = text[static_cast<size_t>(pos)];
// //     return ColoredChar(c, pressed ? bgColor : fgColor);
// // }

