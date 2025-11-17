#include "Menu.h"
#include "Component.h"

#ifndef SUBMENU_H
#define SUBMENU_H

class SubMenu : public Menu {
   private:
    uint32_t x;
    uint32_t y;
	std::vector<Component> components;

   public:
    SubMenu();
    SubMenu(uint32_t w, uint32_t h, uint32_t xCoord, uint32_t yCoord);
    ~SubMenu();
};

inline SubMenu::SubMenu() {
    width = 0;
    height = 0;
    x = 0;
    y = 0;
}

inline SubMenu::SubMenu(uint32_t w, uint32_t h, uint32_t xCoord,
                        uint32_t yCoord) {
    width = w;
    height = h;
    x = xCoord;
    y = yCoord;
}

inline SubMenu::~SubMenu() {
    width = 0;
    height = 0;
    x = 0;
    y = 0;
}

#endif