/**
 * @file    input.hpp
 * @brief   Header file for the Input class.
 * @details This file contains the definition of the Input class which is
 *          responsible for providing an engine-owned input API instead of
 *          exposing direct SDL input checks to game code.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-03
 */

#pragma once

#include <SDL3/SDL.h>

#include <array>
#include <cstddef>

namespace cbit2d::core {

/**
 * @brief Enumerates engine-supported keyboard keys.
 * @details This enum provides a game-facing key list that can grow over time
 *          without forcing gameplay code to depend directly on SDL key codes.
 */
enum class Key : std::size_t {
    Unknown = 0,
    Escape,
    Enter,
    Space,
    Left,
    Right,
    Up,
    Down,
    W,
    A,
    S,
    D,
    F11,
    Count
};

/**
 * @brief Enumerates engine-supported mouse buttons.
 */
enum class MouseButton : std::size_t {
    Left = 0,
    Middle,
    Right,
    Count
};

/**
 * @brief Provides engine-owned keyboard and mouse input state.
 * @details The application layer forwards SDL events into this class, while
 *          game code reads key and pointer state through `cbit2d::core::Input`.
 */
class Input {
public:
    /**
     * @brief Clears per-frame pressed and released input state.
     */
    static void beginFrame();

    /**
     * @brief Processes one SDL event and updates engine input state.
     * @param event SDL event to translate into engine input state.
     */
    static void processEvent(const SDL_Event& event);

    /**
     * @brief Returns whether a key was pressed during the current frame.
     * @param key Engine key to query.
     * @return `true` when the key was pressed this frame, otherwise `false`.
     */
    static bool isKeyPressed(Key key);

    /**
     * @brief Returns whether a key is currently held down.
     * @param key Engine key to query.
     * @return `true` when the key is held, otherwise `false`.
     */
    static bool isKeyHeld(Key key);

    /**
     * @brief Returns whether a key was released during the current frame.
     * @param key Engine key to query.
     * @return `true` when the key was released this frame, otherwise `false`.
     */
    static bool isKeyReleased(Key key);

    /**
     * @brief Returns whether a mouse button was pressed during the current frame.
     * @param button Mouse button to query.
     * @return `true` when the button was pressed this frame, otherwise `false`.
     */
    static bool isMouseButtonPressed(MouseButton button);

    /**
     * @brief Returns whether a mouse button is currently held down.
     * @param button Mouse button to query.
     * @return `true` when the button is held, otherwise `false`.
     */
    static bool isMouseButtonHeld(MouseButton button);

    /**
     * @brief Returns whether a mouse button was released during the current frame.
     * @param button Mouse button to query.
     * @return `true` when the button was released this frame, otherwise `false`.
     */
    static bool isMouseButtonReleased(MouseButton button);

    /**
     * @brief Returns the current mouse position in window coordinates.
     * @return Current cached mouse position.
     */
    static SDL_FPoint getMousePosition();

private:
    /**
     * @brief Converts an SDL key code into an engine key.
     * @param keycode SDL key code to convert.
     * @return Matching engine key.
     */
    static Key mapKeycode(SDL_Keycode keycode);

    /**
     * @brief Converts an engine key into an array index.
     * @param key Engine key to convert.
     * @return Zero-based index for internal state arrays.
     */
    static std::size_t toIndex(Key key);

    /**
     * @brief Converts an SDL mouse button into an engine mouse button.
     * @param button SDL mouse button value to convert.
     * @return Matching engine mouse button.
     */
    static MouseButton mapMouseButton(Uint8 button);

    /**
     * @brief Converts an engine mouse button into an array index.
     * @param button Engine mouse button to convert.
     * @return Zero-based index for internal mouse state arrays.
     */
    static std::size_t toIndex(MouseButton button);

    static std::array<bool, static_cast<std::size_t>(Key::Count)> _pressedKeys;
    static std::array<bool, static_cast<std::size_t>(Key::Count)> _heldKeys;
    static std::array<bool, static_cast<std::size_t>(Key::Count)> _releasedKeys;
    static std::array<bool, static_cast<std::size_t>(MouseButton::Count)> _pressedMouseButtons;
    static std::array<bool, static_cast<std::size_t>(MouseButton::Count)> _heldMouseButtons;
    static std::array<bool, static_cast<std::size_t>(MouseButton::Count)> _releasedMouseButtons;
    static SDL_FPoint _mousePosition;
};

} // namespace cbit2d::core
