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

#include "../ColoredChar/ColoredChar.h"
#include "../Component/Component.h"

/**
 * @brief Represents a menu object. Holds components and handles rendering.
 */
class Menu {
   private:
    std::vector<std::unique_ptr<Component>> components;  // Components in the menu to render,
                                         // first component is bottommost
    std::vector<std::vector<ColoredChar>> renderBuffer;  // Render buffer for
                                                         // the menu

   protected:
    uint32_t width;
    uint32_t height;

   public:
   /**
    * @brief Construct a new default constructed Menu object
    * 
    */
    Menu() = default;

    /**
     * @brief Construct a new Menu object. Creates a frame around the menu.
     *
     * @param w width of menu
     * @param h height of menu
     */
    Menu(uint32_t w, uint32_t h);

    /**
     * @brief Destroy the Menu object.
     *
     */
    ~Menu();

    /**
     * @brief Adds a component to the menu.
     *
     * @param c Component to add
     */
    void addComponent(std::unique_ptr<Component> c) { components.emplace_back(std::move(c)); }

    /**
     * @brief Render the menu by rendering all components into the render
     * buffer.
     *
     * Elements are placed inside the frame, frame cannot be overwritten.
     * Elements are rendered in the order they were added, first added is
     * bottommost.
     *
     */
    void render();
};
