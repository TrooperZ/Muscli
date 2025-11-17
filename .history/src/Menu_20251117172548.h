/**
 * @file Menu.h
 * @author Amin Karic
 * @brief Menu class definition
 * @version 0.1
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 * This class is the big container for the music player. It handles rendering
 * the components that make up each menu screen.
 */
#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#include "ColoredChar/ColoredChar.h"
#include "Component/Component.h"

/**
 * @brief Represents a menu object. Holds components and handles rendering.
 */
class Menu {
   private:
    std::vector<Component*> components;  // Components in the menu to render,
                                         // first component is bottommost
    std::vector<std::vector<ColoredChar>> renderBuffer;  // Render buffer for
                                                         // the menu

   protected:
    uint32_t width;
    uint32_t height;

   public:
    Menu() = default;
    Menu(uint32_t w, uint32_t h);
    ~Menu();
    void addComponent(Component* c);
    void render();
};
/**
 * @brief Construct a new Menu object. Creates a frame around the menu.
 *
 * @param w width of menu
 * @param h height of menu
 */

inline Menu::Menu(uint32_t w, uint32_t h) : width(w), height(h) {
    // Initialize render buffer size
    renderBuffer.resize(h, std::vector<ColoredChar>(w));
    for (auto& row : renderBuffer) {
        row.resize(w);
    }

    // Build frame around menu
    renderBuffer[0][0] = ColoredChar(U'┌', CCHAR_WHITE);
    renderBuffer[0][width - 1] = ColoredChar(U'┐', CCHAR_WHITE);
    renderBuffer[height - 1][0] = ColoredChar(U'└', CCHAR_WHITE);
    renderBuffer[height - 1][width - 1] = ColoredChar(U'┘', CCHAR_WHITE);

    for (auto i = 1; i < width - 1; ++i) {
        renderBuffer[0][i] = ColoredChar(U'─', CCHAR_WHITE);
        renderBuffer[height - 1][i] = ColoredChar(U'─', CCHAR_WHITE);
    }

    for (auto i = 1; i < height - 1; ++i) {
        renderBuffer[i][0] = ColoredChar(U'│', CCHAR_WHITE);
        for (auto j = 1; j < width - 1; ++j) {
            renderBuffer[i][j] = ColoredChar(U' ', CCHAR_WHITE);
        }
        renderBuffer[i][width - 1] = ColoredChar(U'│', CCHAR_WHITE);
    }
}

/**
 * @brief Render the menu by rendering all components into the render buffer.
 *
 * Elements are placed inside the frame, frame cannot be overwritten. Elements
 * are rendered in the order they were added, first added is bottommost.
 *
 */
inline void Menu::render() {
    // Put the updated components into the render buffer
    for (const auto& comp : components) {
        // Render the component pixel by pixel
        for (uint32_t y = 0; y < comp->getHeight(); ++y) {
            for (uint32_t x = 0; x < comp->getWidth(); ++x) {
                // We don't want anything on the border, so we add an offset of
                // 1
                int bufX = comp->getX() + x + 1;
                int bufY = comp->getY() + y + 1;

                // Range check, only render if inside the buffer
                if (bufX >= 0 && bufX < width && bufY >= 0 && bufY < height) {
                    renderBuffer[bufY][bufX] = comp->pixelAt(x, y);
                }
            }
        }
    }

    // Output the render buffer to the console.
    // TODO: Change this to a ostream? ncurses?
    for (const auto& row : renderBuffer) {
        for (const auto& pixel : row) {
            std::cout << pixel;
        }
        std::cout << '\n';
    }
}

/**
 * @brief Adds a component to the menu.
 *
 * @param c Component to add
 */
inline void Menu::addComponent(Component* c) { components.push_back(c); }

/**
 * @brief Destroy the Menu object.
 *
 */
inline Menu::~Menu() {
    width = 0;
    height = 0;
    components.clear();
    renderBuffer.clear();
}
