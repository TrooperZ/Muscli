/**
 * @file TextInput.h
 * @brief Scoped terminal settings and timed keyboard input.
 */
#pragma once

#include <termios.h>

class TextInput {
   private:
    struct termios original {};

   public:
    TextInput();
    ~TextInput();
    TextInput(const TextInput&) = delete;
    TextInput& operator=(const TextInput&) = delete;

    // Returns a key, -1 on timeout/interruption, or -2 when stdin closes.
    int readKey(int timeoutMs);
};
