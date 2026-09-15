/**
 * @file Renderer.cpp
 * @brief Draws the menu on the application thread.
 */
#include "Renderer.h"

void Renderer::draw() {
    Menu* targetMenu = &menu;
    size_t menuWidth = targetMenu->getWidth();
    size_t menuHeight = targetMenu->getHeight();

    if (menuWidth < 2 || menuHeight < 2) return;
    outputBuffer.assign(menuHeight,
                        std::vector<ColoredChar>(menuWidth, BLANK_CHARACTER));

    // Draw corners
    outputBuffer[0][0] = ColoredChar(U'┌', CCHAR_WHITE);
    outputBuffer[0][menuWidth - 1] = ColoredChar(U'┐', CCHAR_WHITE);
    outputBuffer[menuHeight - 1][0] = ColoredChar(U'└', CCHAR_WHITE);
    outputBuffer[menuHeight - 1][menuWidth - 1] =
        ColoredChar(U'┘', CCHAR_WHITE);

    // Draw top and bottom edges
    for (size_t i = 1; i < menuWidth - 1; ++i) {
        outputBuffer[0][i] = ColoredChar(U'─', CCHAR_WHITE);
        outputBuffer[menuHeight - 1][i] = ColoredChar(U'─', CCHAR_WHITE);
    }

    // Draw left and right edges
    for (size_t i = 1; i < menuHeight - 1; ++i) {
        outputBuffer[i][0] = ColoredChar(U'│', CCHAR_WHITE);
        outputBuffer[i][menuWidth - 1] = ColoredChar(U'│', CCHAR_WHITE);
    }

    // Put the updated components into the render buffer
    for (const auto& comp : targetMenu->getComponents()) {
        for (uint32_t y = 0; y < comp->getHeight(); ++y) {
            for (uint32_t x = 0; x < comp->getWidth(); ++x) {
                // Objects are placed inside the frame, so offset by 1
                size_t bufX = comp->getX() + x + 1;
                size_t bufY = comp->getY() + y + 1;

                // Range check, only render if inside the buffer
                if (bufX > 0 && bufX < menuWidth - 1 && bufY > 0 &&
                    bufY < menuHeight - 1) {
                    outputBuffer[bufY][bufX] = comp->pixelAt(x, y);
                }
            }
        }
    }

    std::cout << "\x1b[H";  // Return to the top without clearing scrollback

    for (const auto& row : outputBuffer) {
        for (const auto& pixel : row) {
            std::cout << pixel;
        }
        std::cout << '\n';
    }

    std::cout << std::flush;
}
