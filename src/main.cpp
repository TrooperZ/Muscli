#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "AudioPlayer/AudioPlayer.h"
#include "Component/AlbumAsciiArt/AlbumAsciiArt.h"
#include "Component/SeekBar/SeekBar.h"
#include "Component/Text/Text.h"
#include "Menu/Menu.h"
#include "Renderer/Renderer.h"
#include "TextInput/InputState/InputState.h"
#include "TextInput/TextInput.h"

static volatile sig_atomic_t interrupted = 0;
static void requestStop(int) { interrupted = 1; }

static std::string formatTime(float seconds) {
    uint32_t total = static_cast<uint32_t>(std::max(0.0f, seconds));
    return std::to_string(total / 60) + ":" +
           (total % 60 < 10 ? "0" : "") + std::to_string(total % 60);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: muscli <audio-file>\n";
        return 1;
    }
    try {
        std::filesystem::path filepath(argv[1]);
        if (!std::filesystem::is_regular_file(filepath)) {
            throw std::runtime_error("Audio file does not exist or is not a regular file");
        }
        struct sigaction action{};
        action.sa_handler = requestStop;
        sigemptyset(&action.sa_mask);
        for (int signal : {SIGINT, SIGTERM, SIGHUP}) {
            if (sigaction(signal, &action, nullptr) == -1) {
                throw std::runtime_error("Could not install shutdown handler");
            }
        }

        uint32_t width = 80;
        uint32_t height = 24;
        Menu menu(width, height - 1);
        Menu* m = &menu;

        // ponytail: ASCII filename display until Unicode cell widths are supported
        std::string filename = filepath.filename().string();
        for (char& c : filename) {
            unsigned char byte = static_cast<unsigned char>(c);
            if (byte < 32 || byte >= 127) c = '?';
        }
        m->addComponent(std::make_unique<Text>(40, 5, filename.substr(0, 36), 255, 255, 255));
        m->addComponent(std::make_unique<Text>(40, 6, "Local file", 255, 255, 255));
        m->addComponent(std::make_unique<Text>(41, 13, "S", 255, 255, 255));
        m->addComponent(std::make_unique<Text>(48, 13, "<<", 255, 255, 255));
        auto playback = std::make_unique<Text>(55, 13, "||", 255, 255, 255);
        Text* playbackPtr = playback.get();
        m->addComponent(std::move(playback));
        m->addComponent(std::make_unique<Text>(62, 13, ">>", 255, 255, 255));
        m->addComponent(std::make_unique<Text>(70, 13, "L", 255, 255, 255));
        auto progress = std::make_unique<SeekBar>(40, 11, 30, 0);
        SeekBar* progressPtr = progress.get();
        m->addComponent(std::move(progress));
        auto time = std::make_unique<Text>(40, 10, "0:00 / --:--", 255, 255, 255);
        Text* timePtr = time.get();
        m->addComponent(std::move(time));

        // Keep the existing demo artwork in its original position
        std::filesystem::path artwork = std::filesystem::path(argv[0]).parent_path() / "starboy.png";
        if (!std::filesystem::exists(artwork)) artwork = "src/starboy.png";
        if (std::filesystem::exists(artwork)) {
            m->addComponent(std::make_unique<AlbumAsciiArt>(artwork.string(), 5, 3));
        }
        auto status = std::make_unique<Text>(40, 20, "Playing", 255, 255, 255);
        Text* statusPtr = status.get();
        m->addComponent(std::move(status));

        InputState inputState{};
        Renderer renderer(inputState, {m});
        TextInput textInput;
        AudioPlayer player(filepath.string());
        float duration = player.getDuration();
        std::string previousState;
        uint32_t previousWidth = 0, previousHeight = 0;
        while (!interrupted) {
            struct winsize terminal{};
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal) == -1 ||
                terminal.ws_col == 0 || terminal.ws_row == 0) {
                throw std::runtime_error("Could not read terminal dimensions");
            }
            float position = player.getPosition();
            bool finished = player.isFinished();
            std::string state = finished ? "Finished" : player.isPlaying() ? "Playing" : "Paused";
            std::string timeText = formatTime(finished && duration > 0 ? duration : position) +
                                   " / " + (duration > 0 ? formatTime(duration) : "--:--");
            uint8_t percentage = duration > 0 ? static_cast<uint8_t>(
                std::clamp(position / duration * 100.0f, 0.0f, 100.0f)) : 0;
            if (finished) percentage = 100;
            std::string currentState = state + timeText + std::to_string(percentage) + inputState.buffer;
            if (currentState != previousState || terminal.ws_col != previousWidth || terminal.ws_row != previousHeight) {
                if (terminal.ws_col < width || terminal.ws_row < height) {
                    std::string message = "Resize terminal to 80x24 | Space: pause | q: quit";
                    std::cout << "\x1b[2J\x1b[H" << message.substr(0, terminal.ws_col - 1) << std::flush;
                } else {
                    timePtr->rebuildFromString(timeText);
                    progressPtr->setProgress(percentage);
                    playbackPtr->rebuildFromString(player.isPlaying() && !finished ? "||" : ">");
                    statusPtr->rebuildFromString(state);
                    renderer.drawOnce();
                }
                previousState = currentState;
                previousWidth = terminal.ws_col;
                previousHeight = terminal.ws_row;
            }
            int key = textInput.readKey(100);
            if (key == 'q' || key == -2) break;
            if (key == ' ') {
                player.togglePause();
            } else if (key == 127 || key == 8) {
                if (!inputState.buffer.empty()) inputState.buffer.pop_back();
            } else if (key >= 33 && key <= 126 && inputState.buffer.size() < width - 2) {
                inputState.buffer += static_cast<char>(key);
            }
            inputState.cursor = inputState.buffer.size();
        }
    } catch (const std::exception& error) {
        std::cerr << "muscli: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
