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