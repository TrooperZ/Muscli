/**
 * @file InputState.h
 * @author Amin Karic
 * @brief InputState shared object for TextInput
 * @date 2025-12-23
 *
 * @copyright Copyright (c) 2025
 *
 * @details
 * InputState represents the mutable state of the command input area.
 * It is shared between input handling code (such as TextInput) and the
 * processing code (such as Renderer). The renderer is responsible for drawing
 * the input buffer and cursor, while input-handling threads are responsible for
 * mutating this state.
 *
 * InputState itself performs no synchronization and only holds data.
 */
#pragma once

#include <string>

/**
 * @brief The InputState shared object containing data about the TextInput
 *
 * @note This struct contains only state. It does not perform rendering, input
 * handling, or synchronization.
 */
struct InputState {
    std::string buffer;  // Contents in the TextInput
    size_t cursor = 0;   // Position of cursor in TextInput

    // InputState is non-copyable and non-movable to ensure only 1 exists
    InputState(const InputState& other) = delete;
    InputState& operator=(InputState const& other) = delete;
    InputState(InputState&& other) noexcept = delete;
    InputState& operator=(InputState&& other) noexcept = delete;
};