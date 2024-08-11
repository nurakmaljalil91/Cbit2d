/**
 * @file Input.cpp
 * @brief Implementation file for the Input class.
 *
 * This file contains the implementation of the Input class which is responsible for handling input in the game.
 * The Input class is responsible for handling keyboard and mouse input in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-26

 */

#include "Input.h"

void Input::update() {
    _keyPressed.clear();
    _keyReleased.clear();

    _mouseButtonPressed.clear();
    _mouseButtonReleased.clear();

    _prevMouseX = _mouseX;
    _prevMouseY = _mouseY;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                if (!_keyHeld[event.key.keysym.scancode]) { // Prevent repeated key press events
                    _keyPressed[event.key.keysym.sym] = true;
                }
                _keyHeld[event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                _keyReleased[event.key.keysym.sym] = true;
                _keyHeld[event.key.keysym.scancode] = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!_mouseButtonHeld[event.button.button]) { // Prevent repeated mouse button press events
                    _mouseButtonPressed[event.button.button] = true;
                }
                _mouseButtonHeld[event.button.button] = true;
                break;
            case SDL_MOUSEBUTTONUP:
                _mouseButtonReleased[event.button.button] = true;
                _mouseButtonHeld[event.button.button] = false;
                break;
            case SDL_MOUSEMOTION:
                _mouseX = event.motion.x;
                _mouseY = event.motion.y;
                _mouseDeltaX = _mouseX - _prevMouseX;
                _mouseDeltaY = _mouseY - _prevMouseY;
                break;
            case SDL_QUIT:
                _keyPressed[SDLK_ESCAPE] = true;
                break;
            default:
                break;
        }
    }
}

bool Input::isKeyPressed(int key) const {
    auto it = _keyPressed.find(key);
    return it != _keyPressed.end() && it->second;
}

bool Input::isKeyReleased(int key) const {
    auto it = _keyReleased.find(key);
    return it != _keyReleased.end() && it->second;
}

bool Input::isKeyHeld(int key) const {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key); // Convert key to scancode
    auto it = _keyHeld.find(scancode);
    return it != _keyHeld.end() && it->second;
}

bool Input::isQuit() const {
    return isKeyPressed(SDLK_ESCAPE);
}

bool Input::isMouseButtonPressed(int button) const {
    auto it = _mouseButtonPressed.find(button);
    return it != _mouseButtonPressed.end() && it->second;
}

bool Input::isMouseButtonReleased(int button) const {
    auto it = _mouseButtonReleased.find(button);
    return it != _mouseButtonReleased.end() && it->second;
}

bool Input::isMouseButtonHeld(int button) const {
    auto it = _mouseButtonHeld.find(button);
    return it != _mouseButtonHeld.end() && it->second;
}

void Input::getMousePosition(int &x, int &y) const {
    x = _mouseX;
    y = _mouseY;
}

void Input::getMouseDelta(int &dx, int &dy) const {
    dx = _mouseDeltaX;
    dy = _mouseDeltaY;
}