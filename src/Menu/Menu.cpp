/**
 * @file Menu.cpp
 * @author Amin Karic
 * @brief Implementation of Menu class
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Menu.h"

Menu::Menu(uint32_t w, uint32_t h) : width(w), height(h) {
    // Initialize render buffer size
    renderBuffer.resize(h, std::vector<ColoredChar>(w));

    drawBorderFrame();
}

void Menu::drawBorderFrame() {
    // Draw corners
    renderBuffer[0][0] = ColoredChar(U'┌', CCHAR_WHITE);
    renderBuffer[0][width - 1] = ColoredChar(U'┐', CCHAR_WHITE);
    renderBuffer[height - 1][0] = ColoredChar(U'└', CCHAR_WHITE);
    renderBuffer[height - 1][width - 1] = ColoredChar(U'┘', CCHAR_WHITE);

    // Draw top and bottom edges
    for (auto i = 1; i < width - 1; ++i) {
        renderBuffer[0][i] = ColoredChar(U'─', CCHAR_WHITE);
        renderBuffer[height - 1][i] = ColoredChar(U'─', CCHAR_WHITE);
    }

    // Draw left and right edges
    for (auto i = 1; i < height - 1; ++i) {
        renderBuffer[i][0] = ColoredChar(U'│', CCHAR_WHITE);
        renderBuffer[i][width - 1] = ColoredChar(U'│', CCHAR_WHITE);
    }
}

void Menu::render() {
    clearRenderBuffer();

    // Put the updated components into the render buffer
    for (const auto& comp : components) {
        render(comp.get());
    }
}

void Menu::render(Component* comp) {
    // Render the component pixel by pixel
    for (uint32_t y = 0; y < comp->getHeight(); ++y) {
        for (uint32_t x = 0; x < comp->getWidth(); ++x) {
            // Objects are placed inside the frame, so offset by 1
            int bufX = comp->getX() + x + 1;
            int bufY = comp->getY() + y + 1;

            // Range check, only render if inside the buffer
            if (bufX > 0 && bufX < width - 1 && bufY > 0 && bufY < height - 1) {
                renderBuffer[bufY][bufX] = comp->pixelAt(x, y);
            }
        }
    }
}

void Menu::clearRenderBuffer() {
    for (uint32_t y = 1; y < height - 1; ++y) {
        for (uint32_t x = 1; x < width - 1; ++x) {
            renderBuffer[y][x] = BLANK_CHARACTER;
        }
    }
}

bool Menu::removeComponent(size_t index) {
    size_t oldSize = components.size();
    if (index < components.size()) {
        components.erase(components.begin() + index);
    }
    return components.size() != oldSize;
}

bool Menu::removeComponent(Component* comp) {
    size_t oldSize = components.size();
    components.erase(
        std::remove_if(components.begin(), components.end(),
                       [comp](const std::unique_ptr<Component>& ptr) {
                           return ptr.get() == comp;
                       }),
        components.end());
    return components.size() != oldSize;
}