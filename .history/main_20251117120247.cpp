#include <cstdint>
#include <sys/ioctl.h>
#include <unistd.h>
#include "Menu.h"
#include "AlbumAsciiArt.h"
#include "Text.h"

//https://github.com/nothings/stb/tree/master


int getTerminalWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

int getTerminalHeight() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

int main() {
    // uint32_t width = getTerminalWidth();
    // uint32_t height = getTerminalHeight();
    uint32_t width = 80;
    uint32_t height = 24;

    Menu m(width, height - 1);
    m.addComponent(new Text(40, 5, "Starboy", 255, 255, 255 ));
    m.addComponent(new Text(40, 6, "The Weeknd", 255, 255, 255 ));
    m.addComponent(new Text(40, 8, U"⏸", 255, 255, 255 ));
    AlbumAsciiArt *art = new AlbumAsciiArt("sns.png", 5, 3);
    art->AlbumAsciiArt_Test();
    m.addComponent(art);
    m.render();
	while (1) {
		int i = 0; // so it doesnt die
	}

}