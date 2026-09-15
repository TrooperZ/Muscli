/**
 * @file Renderer.cpp
 * @author Amin Karic
 * @brief Renderer implementation file
 * @date 2025-12-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Renderer.h"

bool Renderer::setActive(size_t index) {
    bool set = false;
    {
        std::lock_guard<std::mutex> lock(mtx);

        if (index < menus.size()) {
            set = true;
            dirty = true;
            activeMenu = index;
        }
    }
    if (set) {
        cv.notify_one();  // Use one instead of all because multiple producers
                          // one consumer
    }
    return set;
};
bool Renderer::setActive(Menu* m) {
    bool set = false;
    {
        std::lock_guard<std::mutex> lock(mtx);

        for (auto i = menus.begin(); i != menus.end(); ++i) {
            if (*i == m) {
                activeMenu = i - menus.begin();
                set = true;
                dirty = true;
                break;
            }
        }
    }
    if (set) {
        cv.notify_one();
    }
    return set;
};
bool Renderer::addMenu(Menu* m) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        menus.push_back(m);
        dirty = true;
    }
    cv.notify_one();
    return true;
};
bool Renderer::removeMenu(size_t index) {
    bool found = false;
    {
        std::lock_guard<std::mutex> lock(mtx);

        if (index < menus.size()) {
            found = true;
            dirty = true;
            if (activeMenu == index) {
                activeMenu = 0;
            } else if (activeMenu > index) {
                activeMenu--;
            }
            menus.erase(menus.begin() + index);
        }
    }
    if (found) {
        cv.notify_one();
    }
    return found;
};

bool Renderer::removeMenu(Menu* m) {
    bool found = false;
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto i = menus.begin(); i != menus.end(); ++i) {
            if (*i == m) {
                size_t index = i - menus.begin();
                if (activeMenu == index) {
                    activeMenu = 0;
                } else if (activeMenu > index) {
                    activeMenu--;
                }

                found = true;
                dirty = true;
                menus.erase(i);
                break;
            }
        }
    }
    if (found) {
        cv.notify_one();
    }
    return found;
};

void Renderer::requestRedraw() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        dirty = true;
    }
    cv.notify_one();
}

void Renderer::run() {
    std::unique_lock<std::mutex> lock(mtx);

    while (running) {
        cv.wait(lock, [this] { return dirty || !running; });
        if (!running) {
            break;
        }
        dirty = false;
        lock.unlock();
        draw();
        lock.lock();
    }
};

void Renderer::stop() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
    }
    cv.notify_one();
};

void Renderer::draw() {
    if (activeMenu < menus.size()) {
        Menu* targetMenu = menus[activeMenu];
        size_t menuWidth = targetMenu->getWidth();
                // Reserve one terminal row for input to prevent scrolling
        size_t menuHeight = targetMenu->getHeight() - 1;

        outputBuffer.resize(menuHeight, std::vector<ColoredChar>(menuWidth));

        // Clear buffer
        for (auto& row : outputBuffer) {
            for (auto& c : row) {
                c = BLANK_CHARACTER;
            }
        }

        // Draw corners
        outputBuffer[0][0] = ColoredChar(U'┌', CCHAR_WHITE);
        outputBuffer[0][menuWidth - 1] = ColoredChar(U'┐', CCHAR_WHITE);
        outputBuffer[menuHeight - 1][0] = ColoredChar(U'└', CCHAR_WHITE);
        outputBuffer[menuHeight - 1][menuWidth - 1] =
            ColoredChar(U'┘', CCHAR_WHITE);

        // Draw top and bottom edges
        for (auto i = 1; i < menuWidth - 1; ++i) {
            outputBuffer[0][i] = ColoredChar(U'─', CCHAR_WHITE);
            outputBuffer[menuHeight - 1][i] = ColoredChar(U'─', CCHAR_WHITE);
        }

        // Draw left and right edges
        for (auto i = 1; i < menuHeight - 1; ++i) {
            outputBuffer[i][0] = ColoredChar(U'│', CCHAR_WHITE);
            outputBuffer[i][menuWidth - 1] = ColoredChar(U'│', CCHAR_WHITE);
        }

        // Put the updated components into the render buffer
        for (const auto& comp : targetMenu->getComponents()) {
            for (uint32_t y = 0; y < comp->getHeight(); ++y) {
                for (uint32_t x = 0; x < comp->getWidth(); ++x) {
                    // Objects are placed inside the frame, so offset by 1
                    int bufX = comp->getX() + x + 1;
                    int bufY = comp->getY() + y + 1;

                    // Range check, only render if inside the buffer
                    if (bufX > 0 && bufX < menuWidth - 1 && bufY > 0 &&
                        bufY < menuHeight - 1) {
                        outputBuffer[bufY][bufX] = comp->pixelAt(x, y);
                    }
                }
            }
        }

        std::cout << "\x1b[3J\x1b[2J\x1b[H";  // Clears the screen

        for (const auto& row : outputBuffer) {
            for (const auto& pixel : row) {
                std::cout << pixel;
            }
            std::cout << '\n';
        }

        // ---- Input line handling (FIXES DUPLICATION) ----
        // Input line is directly below the menu
        const size_t inputRow = menuHeight;  // last visible row
        const size_t inputCol = 1;               // start at column 1

        // Move cursor to input line
        std::cout << "\x1b[" << inputRow << ";" << inputCol << "H";

        // Clear the entire input line
        std::cout << "\x1b[2K";

        // Print input buffer
        std::cout << inputState.buffer;

        // Place cursor at end of buffer (simple echo behavior)
        std::cout << "\x1b[" << inputRow << ";"
                  << (inputCol + inputState.buffer.size()) << "H";

        std::cout << std::flush;
    }
};