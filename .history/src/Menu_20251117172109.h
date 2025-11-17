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
    std::vector<Component*> components;
    std::vector<std::vector<ColoredChar>> renderBuffer;

   protected:
    uint32_t width;
    uint32_t height;

   public:
    Menu();
    Menu(uint32_t w, uint32_t h);
    ~Menu();
    void addComponent(Component* c);
    void render();
};

inline Menu::Menu() {
    width = 0;
    height = 0;
}

inline Menu::Menu(uint32_t w, uint32_t h) {
    width = w;
    height = h;
    renderBuffer.resize(h, std::vector<ColoredChar>(w));
    for (auto& row : renderBuffer) {
        row.resize(w);
    }

    // Build frame around menu
    renderBuffer[0][0] = ColoredChar(U'┌', 0xFFFFFFFF);
    renderBuffer[0][width - 1] = ColoredChar(U'┐', 0xFFFFFFFF);
    renderBuffer[height - 1][0] = ColoredChar(U'└', 0xFFFFFFFF);
    renderBuffer[height - 1][width - 1] = ColoredChar(U'┘', 0xFFFFFFFF);

    for (auto i = 1; i < width - 1; ++i) {
        renderBuffer[0][i] = ColoredChar(U'─', 0xFFFFFFFF);
        renderBuffer[height - 1][i] = ColoredChar(U'─', 0xFFFFFFFF);
    }

    for (auto i = 1; i < height - 1; ++i) {
        renderBuffer[i][0] = ColoredChar(U'│', 0xFFFFFFFF);
        for (auto j = 1; j < width - 1; ++j) {
            renderBuffer[i][j] = ColoredChar(U' ', 0xFFFFFFFF);
        }
        renderBuffer[i][width - 1] = ColoredChar(U'│', 0xFFFFFFFF);
    }
}

inline void Menu::render() {
    // Put the updated components into the render buffer
    std::cout << " there are " << components.size()
              << " components to render\n";
    for (const auto& comp : components) {
        std::cout << "Rendering component at (" << comp->getX() << ", "
                  << comp->getY() << ") with size (" << comp->getWidth() << "x"
                  << comp->getHeight() << ")\n";
        for (uint32_t y = 0; y < comp->getHeight(); ++y) {
            for (uint32_t x = 0; x < comp->getWidth(); ++x) {
                int bufX = comp->getX() + x +
                           1;  // We don't want anything on the border
                int bufY = comp->getY() + y + 1;
                if (bufX >= 0 && bufX < width && bufY >= 0 && bufY < height) {
                    renderBuffer[bufY][bufX] = comp->pixelAt(x, y);
                }
            }
        }
    }

    for (const auto& row : renderBuffer) {
        for (const auto& pixel : row) {
            std::cout << pixel;
        }
        std::cout << '\n';
    }
}

inline void Menu::addComponent(Component* c) { components.push_back(c); }

inline Menu::~Menu() {
    width = 0;
    height = 0;
    components.clear();
    renderBuffer.clear();
}
