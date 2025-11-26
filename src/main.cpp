#include <cstdint>
#include <sys/ioctl.h>
#include <memory>
#include <unistd.h>
#include "Menu/Menu.h"
#include "Component/Text/Text.h"
#include "Component/AlbumAsciiArt/AlbumAsciiArt.h"

//https://github.com/nothings/stb/tree/master

// apt-get update && apt-get install -y gcc && apt-get install linux-tools-common   
// int getTerminalWidth() {
//     struct winsize w;
//     ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
//     return w.ws_col;
// }

// int getTerminalHeight() {
//     struct winsize w;
//     ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
//     return w.ws_row;
// }

int main() {
    // uint32_t width = getTerminalWidth();
    // uint32_t height = getTerminalHeight();
    uint32_t width = 80;
    uint32_t height = 24;

    Menu m(width, height - 1);
    m.addComponent(std::make_unique<Text>(40, 5, "Starboy", 255, 255, 255 ));
    m.addComponent(std::make_unique<Text>(40, 6, "The Weeknd", 255, 255, 255 ));
    m.addComponent(std::make_unique<Text>(40, 8, "⏸", 255, 255, 255 ));
    auto art = std::make_unique<AlbumAsciiArt>("starboy.png", 5, 3);
    // art->AlbumAsciiArt_Test();
    m.addComponent(std::unique_ptr<Component>(std::move(art)));
    m.render();
	// while (1) {
	// 	int i = 0; // so it doesnt die
	// }
    return 0;


}