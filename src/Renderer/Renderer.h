/**
 * @file Renderer.h
 * @author your name (you@domain.com)
 * @brief Renderer class definition.
 * @version 0.1
 * @date 2025-12-21
 *
 * @copyright Copyright (c) 2025
 *
 * @details
 * The Renderer class holds multiple menus and is responsible for redrawing the
 * screen when data changes or content switches.
 */

#include <vector>

#include "../Menu/Menu.h"
class Renderer {
   private:
    std::vector<Menu*> menus;  // The different scenes the renderer will hold
    size_t activeMenu;         // Index of the current menu
    bool dirty =
        false;  // State of the render, if dirty, renderer should refresh

   public:
    Renderer() = default;
    Renderer(std::vector<Menu*> menus) : menus(menus), activeMenu(0){};
    Renderer(std::vector<Menu*> menus, size_t activeMenu)
        : menus(menus), activeMenu(activeMenu){};

    Renderer(const Renderer& other) = default;
    Renderer& operator=(Renderer const& other) = default;
    Renderer(Renderer&& other) noexcept = default;
    Renderer& operator=(Renderer&& other) noexcept = default;

    ~Renderer() = default;  // TODO: Maybe write code that clears the console
                            // and displays an exit message since destruction of
                            // renderer usually will mean end of program?

    void setActive(size_t index) { activeMenu = index; }

    void setActive(Menu* m);

    void requestRedraw() { dirty = true; }

	void draw() 

};
