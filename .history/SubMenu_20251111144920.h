
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