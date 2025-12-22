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
 * The Renderer owns the rendering loop and is only responsible
 * for building menus and outputting content to the terminal. Rendering is
 * performed only when requested, allowing background threads to
 * update data without triggering immediate redraws.
 */

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <vector>

#include "../Menu/Menu.h"

/**
 * @class Renderer
 *
 * @brief Manages menu rendering and terminal output.
 *
 * The Renderer holds a vector of menus and controls which menu is
 * currently active. It runs a blocking render loop that sleeps and wakes when
 * notified of data changes.
 *
 * The Renderer is thread-safe and intended to be interacted with by background
 * services via requestRedraw(), while all rendering and terminal output occurs
 * on the renderer thread.
 */
class Renderer {
   private:
    std::vector<Menu*> menus;    // Pointers to Menus rendered by this renderer
    size_t activeMenu;           // Index of the active menu
    bool dirty = false;          // Indicates if redraw requested
    bool running = true;         // Controls the lifetime of the render loop
    std::mutex mtx;              // Protects shared renderer state
    std::condition_variable cv;  // Used to sleep/wake the render loop
    std::vector<std::vector<ColoredChar>>
        outputBuffer;  // TODO: move buffer to renderer

    /**
     * @brief Render and output the active menu once.
     *
     * Generates the menu buffer and writes it to the terminal.
     */
    void draw();

   public:
    Renderer() = default;

    /**
     * @brief Construct a Renderer with a set of menus.
     *
     * The renderer does not take ownership of the menus.
     *
     * @param menus Vector of menu pointers to render
     */
    Renderer(std::vector<Menu*> menus) : menus(menus), activeMenu(0){};

    /**
     * @brief Construct a Renderer with menus and an active menu index.
     *
     * @param menus Vector of menu pointers to render
     * @param activeMenu Index of the initially active menu
     */
    Renderer(std::vector<Menu*> menus, size_t activeMenu)
        : menus(menus), activeMenu(activeMenu >= menus.size() ? menus.size() - 1 : activeMenu){};

    // Renderer is non-copyable and non-movable to enforce single ownership
    Renderer(const Renderer& other) = delete;
    Renderer& operator=(Renderer const& other) = delete;
    Renderer(Renderer&& other) noexcept = delete;
    Renderer& operator=(Renderer&& other) noexcept = delete;

    ~Renderer() = default;

    /**
     * @brief Set the active menu by index.
     *
     * Switching the active menu schedules a redraw.
     *
     * @param index Index of the menu to activate
     * @return bool true if successful, false if fail
     */
    bool setActive(size_t index);

    /**
     * @brief Set the active menu by pointer.
     *
     * The menu must already be managed by the renderer.
     * Switching the active menu schedules a redraw.
     *
     * @param m Pointer to the menu to activate
     * @return bool true if successful, false if fail
     */
    bool setActive(Menu* m);

    /**
     * @brief Add a menu to the renderer
     *
     * Adding a menu schedules a redraw.
     *
     * @param m Menu pointer to add
     * @return bool true if successful, false if fail
     */
    bool addMenu(Menu* m);

    /**
     * @brief Remove a menu from the renderer
     *
     * Removing a menu schedules a redraw.
     *
     * @param index menu to remove by index
     * @return bool true if successful, false if fail
     */
    bool removeMenu(size_t index);

    /**
     * @brief Remove a menu from the renderer
     *
     * The menu must already be managed by the renderer.
     * Removing a menu schedules a redraw.
     *
     * @param m menu to remove by pointer
     * @return bool true if successful, false if fail
     */
    bool removeMenu(Menu* m);

    /**
     * @brief Request that the renderer redraw the active menu.
     *
     */
    void requestRedraw();

    /**
     * @brief Run the renderer loop.
     *
     * Blocks the calling thread until stop() is called. The renderer sleeps
     * when no redraw is requested and wakes when notified.
     */
    void run();

    /**
     * @brief Stop the renderer loop.
     *
     * Wakes the renderer if it is sleeping and causes run() to return.
     */
    void stop();
};
