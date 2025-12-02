/**
 * @file Menu.cpp
 * @author Amin Karic
 * @brief
 * @version 0.1
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025
 *
 * Implementation file for Menu class.
 */

#include <memory>

#include "Menu.h"

/**
 * @brief Construct a new Menu object. Creates a frame around the menu.
 *
 * @param w width of menu
 * @param h height of menu
 */
Menu::Menu(uint32_t w, uint32_t h) : width(w), height(h) {
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
void Menu::render() {
    // Put the updated components into the render buffer
    for (const auto& comp : components) {
        // Render the component pixel by pixel
        for (uint32_t y = 0; y < comp->getHeight(); ++y) {
            for (uint32_t x = 0; x < comp->getWidth(); ++x) {
                // We don't want anything on the border, so we add an offset
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
