#include "Menu.h"

#ifndef SUBMENU_H
#define SUBMENU_H

class SubMenu : extends {
   private:
    uint32_t width;
    uint32_t height;
    std::vector<SubMenu> subMenus;

   public:
    Menu();
    Menu(uint32_t w, uint32_t h);
    ~Menu();
    void addSubMenu();
};

inline Menu::Menu() {
    width = 0;
    height = 0;
}

inline Menu::Menu(uint32_t w, uint32_t h) {
    width = w;
    height = h;
}

inline Menu::~Menu() {
    width = 0;
    height = 0;
    subMenus.clear();
}


#endif