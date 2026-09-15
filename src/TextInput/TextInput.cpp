/**
 * @file TextInput.cpp
 * @brief POSIX terminal input shared by macOS and Linux.
 */
#include "TextInput.h"

#include <poll.h>
#include <unistd.h>

#include <cerrno>
#include <stdexcept>

TextInput::TextInput() {
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        throw std::runtime_error("Playback requires an interactive terminal");
    }
    if (tcgetattr(STDIN_FILENO, &original) == -1) {
        throw std::runtime_error("Could not read terminal settings");
    }
    struct termios raw = original;
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        throw std::runtime_error("Could not configure terminal input");
    }
    const char screen[] = "\x1b[?1049h\x1b[?25l";
    (void)write(STDOUT_FILENO, screen, sizeof(screen) - 1);
}

TextInput::~TextInput() {
    while (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original) == -1 &&
           errno == EINTR) {
    }
    const char screen[] = "\x1b[0m\x1b[?25h\x1b[?1049l";
    (void)write(STDOUT_FILENO, screen, sizeof(screen) - 1);
}

int TextInput::readKey(int timeoutMs) {
    struct pollfd input {
        STDIN_FILENO, POLLIN, 0
    };
    int result = poll(&input, 1, timeoutMs);
    if (result == -1) {
        if (errno == EINTR) return -1;
        throw std::runtime_error("Could not wait for keyboard input");
    }
    if (result == 0) return -1;
    if (input.revents & (POLLERR | POLLNVAL)) {
        throw std::runtime_error("Terminal input failed");
    }
    if (input.revents & POLLIN) {
        unsigned char key;
        ssize_t count = read(STDIN_FILENO, &key, 1);
        if (count == 1) return key;
        if (count == 0) return -2;
        if (errno == EINTR || errno == EAGAIN) return -1;
        throw std::runtime_error("Could not read keyboard input");
    }
    return input.revents & POLLHUP ? -2 : -1;
}
