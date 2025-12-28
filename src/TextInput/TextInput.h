/**
 * @file TextInput.h
 * @author Amin Karic
 * @brief TextInput Class definition
 * @date 2025-12-23
 *
 * @copyright Copyright (c) 2025
 *
 * @details
 * The TextInput class is responsible for handling user command input by
 * consuming stdin. It is intended to be the only stdin consumer. It runs on its
 * own thread alongside the Renderer, allowing the UI to update independently
 * while waiting for input.
 *
 * TextInput does not perform rendering or terminal output. Instead,
 * it mutates shared input state and requests UI redraws from the Renderer
 * when input changes.
 */
#pragma once

#include <atomic>

#include "../Renderer/Renderer.h"
#include "InputState/InputState.h"

/**
 * @brief Enum defining the current mode of key handling
 *
 * HOTKEY: Treat single or combo key presses as quick actions
 * COMMAND: Buffer text input and process upon enter
 * SEARCH: Input for the search menu
 */
enum InputMode { HOTKEY, COMMAND, SEARCH };

/**
 * @class TextInput
 *
 * @brief Class responsible for handling stdin user input
 *
 */
class TextInput {
   private:
    InputState& inputState;  // Object tracking input data
    Renderer& renderer;      // Refrence to renderer holding text input to call
                             // redraws when needed
    std::atomic<bool> running = true;
    InputMode mode = HOTKEY;

   public:
    // Requires refrences therefore we cannot have default ctor
    TextInput() = delete;

    /**
     * @brief Constructs a TextInput instance.
     *
     * @param inputState Reference to the shared input state.
     * @param renderer Reference to the renderer used to request redraws.
     */
    TextInput(InputState& inputState, Renderer& renderer)
        : inputState(inputState), renderer(renderer){};

    // TextInput is non-copyable and non-movable to ensure only 1 exists
    TextInput(const TextInput& other) = delete;
    TextInput& operator=(TextInput const& other) = delete;
    TextInput(TextInput&& other) noexcept = delete;
    TextInput& operator=(TextInput&& other) noexcept = delete;

    /**
     * @brief Entry point for the TextInput thread.
     *
     * @details
     * Starts the TextInput task for processing user input. Saves terminal state
     * and handles input loop. This function is intended to be executed on a
     * dedicated thread.
     */
    void run();

    /**
     * @brief Signals the TextInput thread to stop.
     *
     * @details
     * Causes the input loop to exit cleanly. This function is thread-safe
     * and is typically called during application shutdown.
     */
    void stop();

    /**
     * @brief Processes the current contents of the input buffer.
     *
     * @details
     * This function is invoked when a command has been submitted
     * (presses Enter). It performs the action associated with the contents.
     */
    void processBuffer();
};