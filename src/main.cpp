#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "AudioPlayer/AudioPlayer.h"
#include "Component/SeekBar/SeekBar.h"
#include "Component/Text/Text.h"
#include "Menu/Menu.h"
#include "Renderer/Renderer.h"
#include "TextInput/TextInput.h"

static volatile sig_atomic_t interrupted = 0;
static void requestStop(int) { interrupted = 1; }

static std::string formatTime(float seconds) {
    unsigned int total = static_cast<unsigned int>(std::max(0.0f, seconds));
    return std::to_string(total / 60) + ":" + (total % 60 < 10 ? "0" : "") +
           std::to_string(total % 60);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: muscli <audio-file>\n";
        return 1;
    }
    try {
        std::filesystem::path filepath(argv[1]);
        if (!std::filesystem::is_regular_file(filepath)) {
            throw std::runtime_error(
                "Audio file does not exist or is not a regular file");
        }
        struct sigaction action {};
        action.sa_handler = requestStop;
        sigemptyset(&action.sa_mask);
        for (int signal : {SIGINT, SIGTERM, SIGHUP}) {
            if (sigaction(signal, &action, nullptr) == -1) {
                throw std::runtime_error("Could not install shutdown handler");
            }
        }

        TextInput input;
        AudioPlayer player(filepath.string());
        Menu menu(78, 10);
        auto title = std::make_unique<Text>(1, 0, "", CCHAR_WHITE);
        auto status = std::make_unique<Text>(1, 2, "", CCHAR_WHITE);
        auto time = std::make_unique<Text>(1, 3, "", CCHAR_WHITE);
        auto progress = std::make_unique<SeekBar>(1, 5, 30);
        Text* titlePtr = title.get();
        Text* statusPtr = status.get();
        Text* timePtr = time.get();
        SeekBar* progressPtr = progress.get();
        menu.addComponent(std::move(title));
        menu.addComponent(std::move(status));
        menu.addComponent(std::move(time));
        menu.addComponent(std::move(progress));
        menu.addComponent(std::make_unique<Text>(
            1, 7, "Space: pause/resume/restart | q: quit", CCHAR_WHITE));
        Renderer renderer(menu);

        // ponytail: ASCII filename display; add Unicode cell widths with UTF-8
        // support. Keep filenames from injecting terminal controls or invalid
        // UTF-8.
        std::string filename = filepath.filename().string();
        for (char& c : filename) {
            unsigned char byte = static_cast<unsigned char>(c);
            if (byte < 32 || byte >= 127) c = '?';
        }
        float duration = player.getDuration();
        std::string previousState;
        unsigned int previousWidth = 0, previousHeight = 0;
        while (!interrupted) {
            struct winsize terminal {};
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal) == -1 ||
                terminal.ws_col == 0 || terminal.ws_row == 0) {
                throw std::runtime_error("Could not read terminal dimensions");
            }
            unsigned int width = terminal.ws_col;
            unsigned int height = terminal.ws_row;
            float position = player.getPosition();
            bool finished = player.isFinished();
            std::string state = finished             ? "Finished"
                                : player.isPlaying() ? "Playing"
                                                     : "Paused";
            std::string timeText =
                formatTime(finished && duration > 0 ? duration : position) +
                " / " + (duration > 0 ? formatTime(duration) : "--:--");
            uint8_t percentage =
                duration > 0 ? static_cast<uint8_t>(std::clamp(
                                   position / duration * 100.0f, 0.0f, 100.0f))
                             : 0;
            if (finished) percentage = 100;
            std::string currentState =
                state + timeText + std::to_string(percentage);
            if (currentState != previousState || width != previousWidth ||
                height != previousHeight) {
                if (width != previousWidth || height != previousHeight)
                    std::cout << "\x1b[2J";
                if (width < 42 || height < 12) {
                    std::string compact = state + " " + timeText + " Space/q";
                    std::cout << "\x1b[H\x1b[2K" << compact.substr(0, width - 1)
                              << std::flush;
                } else {
                    menu.setSize(std::min(width - 1, 80u), 10);
                    titlePtr->rebuildFromString(
                        filename.substr(0, menu.getWidth() - 4));
                    statusPtr->rebuildFromString(state);
                    timePtr->rebuildFromString(timeText);
                    progressPtr->setWidth(menu.getWidth() - 4);
                    progressPtr->setProgress(percentage);
                    renderer.draw();
                }
                previousState = currentState;
                previousWidth = width;
                previousHeight = height;
            }
            int key = input.readKey(100);
            if (key == 'q' || key == -2) break;
            if (key == ' ') player.togglePause();
        }
    } catch (const std::exception& error) {
        std::cerr << "muscli: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
