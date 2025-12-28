#include <sys/ioctl.h>
#include <unistd.h>

#include <cstdint>
#include <memory>

// Headers for dummy thread test
#include <atomic>
#include <chrono>
#include <string>
#include <thread>

#include "Component/AlbumAsciiArt/AlbumAsciiArt.h"
#include "Component/SeekBar/SeekBar.h"
#include "Component/Text/Text.h"
#include "Menu/Menu.h"
#include "Renderer/Renderer.h"
#include "TextInput/InputState/InputState.h"
#include "TextInput/TextInput.h"

// https://github.com/nothings/stb/tree/master

// apt-get update && apt-get install -y gcc && apt-get install
// linux-tools-common int getTerminalWidth() {
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

    Menu* m = new Menu(width, height - 1);
    m->addComponent(std::make_unique<Text>(40, 5, "Starboy", 255, 255, 255));
    m->addComponent(std::make_unique<Text>(40, 6, "The Weeknd", 255, 255, 255));
    m->addComponent(std::make_unique<Text>(41, 13, "S", 255, 255, 255));
    m->addComponent(std::make_unique<Text>(48, 13, "<<", 255, 255, 255));
    m->addComponent(std::make_unique<Text>(55, 13, "||", 255, 255, 255));
    m->addComponent(std::make_unique<Text>(62, 13, ">>", 255, 255, 255));
    m->addComponent(std::make_unique<Text>(70, 13, "L", 255, 255, 255));
    m->addComponent(std::unique_ptr<SeekBar>(new SeekBar(40, 11, 30, 70)));
    m->addComponent(
        std::make_unique<Text>(40, 10, "3:15 / 4:20", 255, 255, 255));
    auto art = std::make_unique<AlbumAsciiArt>("starboy.png", 5, 3);
    // art->AlbumAsciiArt_Test();
    m->addComponent(std::unique_ptr<Component>(std::move(art)));

    std::atomic<bool> running{true};

    auto dynamicText =
        std::make_unique<Text>(40, 20, "Initial text", 255, 255, 255);
    Text* dynamicTextPtr = dynamicText.get();
    m->addComponent(std::move(dynamicText));

    InputState inputState{};
    Renderer renderer(inputState, {m});
    TextInput textInput(inputState, renderer);

    std::thread rendererThread([&renderer]() { renderer.run(); });

    std::thread textInputThread([&textInput]() { textInput.run(); });

    std::thread dummyWorker([&]() {
        int counter = 0;

        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(5));

            // Generate dummy text
            std::string msg = "Update #" + std::to_string(counter++);

            // Update shared data (Text object)
            dynamicTextPtr->rebuildFromString(msg);

            // Tell renderer something changed
            renderer.requestRedraw();
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(30));

    running = false;

    renderer.stop();

    dummyWorker.join();
    rendererThread.join();

    // while (1) {
    // 	int i = 0; // so it doesnt die
    // }
    return 0;
}