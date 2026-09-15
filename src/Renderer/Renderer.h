/**
 * @file Renderer.h
 * @brief Renders a menu when the application state changes.
 */
#pragma once

#include <vector>

#include "../Menu/Menu.h"

class Renderer {
   private:
    Menu& menu;
    std::vector<std::vector<ColoredChar>> outputBuffer;

   public:
    explicit Renderer(Menu& m) : menu(m){};
    void draw();
};
