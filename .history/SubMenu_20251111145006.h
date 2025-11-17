#include "Menu.h"

#ifndef SUBMENU_H
#define SUBMENU_H

class SubMenu : public Menu {
   private:
    uint32_t width;
    uint32_t height;
    std::vector<SubMenu> subMenus;
	

   public:
    SubMenu();
    SubMenu(uint32_t w, uint32_t h);
    ~SubMenu();
    
};

inline SubMenu::SubMenu() {
    width = 0;
    height = 0;
}

inline SubMenu::SubMenu(uint32_t w, uint32_t h) {
    width = w;
    height = h;
}

inline SubMenu::~SubMenu() {
    width = 0;
    height = 0;
    subMenus.clear();
}


#endif