/**
 * @file    input.cpp
 * @brief   Source file for the Input class.
 * @details This file contains the implementation of the Input class which is
 *          responsible for translating SDL input events into engine-owned key
 *          state for gameplay code.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-03
 */

#include "cbit/core/input.hpp"

namespace cbit2d::core {

std::array<bool, static_cast<std::size_t>(Key::Count)> Input::_pressedKeys{};
std::array<bool, static_cast<std::size_t>(Key::Count)> Input::_heldKeys{};
std::array<bool, static_cast<std::size_t>(Key::Count)> Input::_releasedKeys{};
std::array<bool, static_cast<std::size_t>(MouseButton::Count)> Input::_pressedMouseButtons{};
std::array<bool, static_cast<std::size_t>(MouseButton::Count)> Input::_heldMouseButtons{};
std::array<bool, static_cast<std::size_t>(MouseButton::Count)> Input::_releasedMouseButtons{};
SDL_FPoint Input::_mousePosition {};

/**
 * @brief Clears the transient input state for the current frame.
 */
void Input::beginFrame()
{
    _pressedKeys.fill(false);
    _releasedKeys.fill(false);
    _pressedMouseButtons.fill(false);
    _releasedMouseButtons.fill(false);

    float mouseX = 0.0F;
    float mouseY = 0.0F;
    SDL_GetMouseState(&mouseX, &mouseY);
    _mousePosition = {mouseX, mouseY};
}

/**
 * @brief Processes one SDL event and updates engine key state.
 * @param event SDL event to translate into engine input state.
 */
void Input::processEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        _mousePosition = {event.motion.x, event.motion.y};
        return;
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        _mousePosition = {event.button.x, event.button.y};

        const MouseButton button = mapMouseButton(event.button.button);
        if (button == MouseButton::Count) {
            return;
        }

        const std::size_t index = toIndex(button);

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            _pressedMouseButtons[index] = true;
            _heldMouseButtons[index] = true;
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            _releasedMouseButtons[index] = true;
            _heldMouseButtons[index] = false;
        }

        return;
    }

    if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) {
        return;
    }

    const Key key = mapKeycode(event.key.key);
    if (key == Key::Unknown) {
        return;
    }

    const std::size_t index = toIndex(key);

    if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
        _pressedKeys[index] = true;
        _heldKeys[index] = true;
    }

    if (event.type == SDL_EVENT_KEY_UP) {
        _releasedKeys[index] = true;
        _heldKeys[index] = false;
    }
}

/**
 * @brief Returns whether a key was pressed during the current frame.
 * @param key Engine key to query.
 * @return `true` when the key was pressed this frame, otherwise `false`.
 */
bool Input::isKeyPressed(Key key)
{
    return _pressedKeys[toIndex(key)];
}

/**
 * @brief Returns whether a key is currently held down.
 * @param key Engine key to query.
 * @return `true` when the key is held, otherwise `false`.
 */
bool Input::isKeyHeld(Key key)
{
    return _heldKeys[toIndex(key)];
}

/**
 * @brief Returns whether a key was released during the current frame.
 * @param key Engine key to query.
 * @return `true` when the key was released this frame, otherwise `false`.
 */
bool Input::isKeyReleased(Key key)
{
    return _releasedKeys[toIndex(key)];
}

/**
 * @brief Returns whether a mouse button was pressed during the current frame.
 * @param button Mouse button to query.
 * @return `true` when the button was pressed this frame, otherwise `false`.
 */
bool Input::isMouseButtonPressed(MouseButton button)
{
    return _pressedMouseButtons[toIndex(button)];
}

/**
 * @brief Returns whether a mouse button is currently held down.
 * @param button Mouse button to query.
 * @return `true` when the button is held, otherwise `false`.
 */
bool Input::isMouseButtonHeld(MouseButton button)
{
    return _heldMouseButtons[toIndex(button)];
}

/**
 * @brief Returns whether a mouse button was released during the current frame.
 * @param button Mouse button to query.
 * @return `true` when the button was released this frame, otherwise `false`.
 */
bool Input::isMouseButtonReleased(MouseButton button)
{
    return _releasedMouseButtons[toIndex(button)];
}

/**
 * @brief Returns the current mouse position in window coordinates.
 * @return Current cached mouse position.
 */
SDL_FPoint Input::getMousePosition()
{
    return _mousePosition;
}

/**
 * @brief Converts an SDL key code into an engine key.
 * @param keycode SDL key code to convert.
 * @return Matching engine key.
 */
Key Input::mapKeycode(SDL_Keycode keycode)
{
    switch (keycode) {
    case SDLK_ESCAPE:
        return Key::Escape;
    case SDLK_RETURN:
        return Key::Enter;
    case SDLK_SPACE:
        return Key::Space;
    case SDLK_LEFT:
        return Key::Left;
    case SDLK_RIGHT:
        return Key::Right;
    case SDLK_UP:
        return Key::Up;
    case SDLK_DOWN:
        return Key::Down;
    case SDLK_W:
        return Key::W;
    case SDLK_A:
        return Key::A;
    case SDLK_S:
        return Key::S;
    case SDLK_D:
        return Key::D;
    case SDLK_F11:
        return Key::F11;
    default:
        return Key::Unknown;
    }
}

/**
 * @brief Converts an engine key into an internal array index.
 * @param key Engine key to convert.
 * @return Zero-based index for internal state arrays.
 */
std::size_t Input::toIndex(Key key)
{
    return static_cast<std::size_t>(key);
}

/**
 * @brief Converts an SDL mouse button into an engine mouse button.
 * @param button SDL mouse button value to convert.
 * @return Matching engine mouse button.
 */
MouseButton Input::mapMouseButton(Uint8 button)
{
    switch (button) {
    case SDL_BUTTON_LEFT:
        return MouseButton::Left;
    case SDL_BUTTON_MIDDLE:
        return MouseButton::Middle;
    case SDL_BUTTON_RIGHT:
        return MouseButton::Right;
    default:
        return MouseButton::Count;
    }
}

/**
 * @brief Converts an engine mouse button into an internal array index.
 * @param button Engine mouse button to convert.
 * @return Zero-based index for internal state arrays.
 */
std::size_t Input::toIndex(MouseButton button)
{
    return static_cast<std::size_t>(button);
}

} // namespace cbit2d::core
