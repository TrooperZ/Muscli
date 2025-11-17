#include "Menu.h"

#ifndef SUBMENU_H
#define SUBMENU_H

class SubMenu : public Menu {
   private:
	uint32_t x;
	uint32_t y;
	

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
	x = 0;
	y = 0;
}


#endif