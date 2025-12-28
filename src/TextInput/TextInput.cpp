/**
 * @file TextInput.cpp
 * @author Amin Karic
 * @brief TextInput class definition
 * @date 2025-12-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "TextInput.h"

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html

static struct termios orig_termios;  // Original terminal state

// Helper functions since atexit() requires (*)(void)
void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void TextInput::run() {
    // Enable raw mode and save terminal state
    tcgetattr(STDIN_FILENO, &orig_termios);

    // Restores state at program exit
    atexit(disableRawMode);

    struct termios raw = orig_termios;

    raw.c_iflag &= ~(ICRNL | IXON);  // Disable Ctrl-S and Ctrl-Q and Ctrl-M
    // raw.c_oflag &= ~(OPOST); // disable output processing with carrige
    // returns, maybe keep this on because it looks nicer and easier?
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
    // ECHO: echo flag (disables)
    // ICANON: Canonical mode disabled, allows for raw input
    // IEXTEN: disables ctrl-v input
	// It is reccomended to also disable BRKINT, INPCK, ISTRIP, and CS8 as per the tutorial to keep the "raw mode tradition" but I will keep them on
    // TODO: Add the ISIG parameter to this to disable Ctrl C and ctrl z (or
    // ctrl y on macos)

    // * TCSAFLUSH: waits for all pending output to be written to the terminal,
    // * and also discards any input that hasn't been read.

    tcsetattr(STDIN_FILENO, TCSAFLUSH,
              &raw);  // Modify attributes to enable raw mode

    running = true;
    while (running) {
        // check if key == space, print
        // if key == q, exit()
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 'q'){
				exit(1);
			} else {
				inputState.buffer += c;
                inputState.cursor++;
				renderer.requestRedraw();
			}
        }
    }
    // restore terminal state
}
void TextInput::stop() { running = false; }
void TextInput::processBuffer() {
    std::cout << inputState.buffer << " Command processed!\n";
}