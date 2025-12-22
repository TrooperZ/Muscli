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
        targetMenu->render();

        // TODO: Change to looping to have renderer do the thing since the
        // buffer will live on the renderer in the future
        std::cout << targetMenu;
    }
};