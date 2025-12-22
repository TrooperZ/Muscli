/**
 * @file Menu.h
 * @author Amin Karic
 * @brief Menu class definition
 * @date 2025-11-17
 *
 * @copyright Copyright (c) 2025
 *
 * @details
 * This class is the top-level container for the music player UI. It handles
 * rendering the components that make up each menu screen.
 */
#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>

#include "../ColoredChar/ColoredChar.h"
#include "../Component/Component.h"

/**
 * @class Menu
 *
 * @brief Represents a menu object. Holds components and handles rendering.
 */
class Menu {
   private:
    std::vector<std::unique_ptr<Component>>
        components;  // Components in the menu to render,
                     // first component is bottommost
    std::vector<std::vector<ColoredChar>> renderBuffer;  // 2D Render buffer for
                                                         // the menu

   protected:
    uint32_t width;
    uint32_t height;

   public:
    Menu() = delete;  // No default constructor because width and height are
                      // required to draw the frame around the menu

    /**
     * @brief Construct a new Menu object given width and height.
     *
     * @param w width of menu
     * @param h height of menu
     *
     * @details
     * Creates a bordered frame and initializes the render buffer to the
     * given dimensions.
     */
    Menu(uint32_t w, uint32_t h);

    Menu(const Menu& other) = default;
    Menu& operator=(Menu const& other) = default;
    Menu(Menu&& other) noexcept = default;
    Menu& operator=(Menu&& other) noexcept = default;

    ~Menu() = default;

    uint32_t getWidth() const noexcept { return width; }
    uint32_t getHeight() const noexcept { return height; }

    /**
     * @brief Adds a component to the menu.
     *
     * @param c Component to add. Ownership is transferred to the menu.
     *
     * @details
     * This does not redraw the buffer. You must manually call a redraw.
     */
    void addComponent(std::unique_ptr<Component> c) {
        components.emplace_back(std::move(c));
    }

    /**
     * @brief Removes and deletes a component from the menu given an index.
     *
     * @param index index of the component to remove
     * @return true deletion occured
     * @return false no deletion occured
     *
     * @details
     * This does not redraw the buffer. You must manually call a redraw.
     */
    bool removeComponent(size_t index);

    /**
     * @brief Removes and deletes a component from the menu given a pointer.
     *
     * @param comp pointer of the desired component to remove
     * @return true deletion occured
     * @return false no deletion occured
     *
     * @details
     * This does not redraw the buffer. You must manually call a redraw.
     */
    bool removeComponent(Component* comp);

    /**
     * @brief Clears all components from the menu.
     *
     * @details
     * This does not redraw the buffer. You must manually call a redraw.
     */
    void clearComponents() { components.clear(); }

    /**
     * @brief Render the menu by drawing all components into the render
     * buffer.
     *
     * @details
     * Elements are placed inside the frame, frame cannot be overwritten.
     * Elements are rendered in the order they were added, first added is
     * bottommost.
     */
    void render();

    /**
     * @brief Renders a single component into the render buffer.
     *
     * @param comp Component to render.
     *
     * @details
     * This function draws the given component into the render buffer at its
     * specified position. It does not clear or modify other parts of the
     * render buffer. Useful for updating only a single component in the menu.
     */
    void render(Component* comp);

    /**
     * @brief Draws the border frame around the menu onto the render buffer.
     *
     * @details
     * This function initializes the corners and edges of the menu frame. Useful
     * if we ever need to redraw the frame.
     */
    void drawBorderFrame();

    /**
     * @brief Clears the render buffer inside the border frame.
     *
     */
    void clearRenderBuffer();

    /**
     * @brief Stream insertion operator overload for Menu. Outputs the buffer.
     *
     * @param os output stream
     * @param menu Menu to output
     * @return std::ostream& output stream with menu buffer contents
     */
    // TODO: Change this to ncurses?
    friend std::ostream& operator<<(std::ostream& os, const Menu& menu) {
        for (const auto& row : menu.renderBuffer) {
            for (const auto& pixel : row) {
                os << pixel;
            }
            os << '\n';
        }
        return os;
    }
};
