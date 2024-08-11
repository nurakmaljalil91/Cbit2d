/**
 * @file Input.h
 * @brief Input class header file
 *
 * This file contains the definition of the Input class which is responsible for handling input in the game.
 * The Input class is responsible for handling keyboard and mouse input in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-26
 */

#ifndef CBIT2D_INPUT_H
#define CBIT2D_INPUT_H

#include <SDL2/SDL.h>
#include <unordered_map>

class Input {
public:
    // Updates the input state; should be called once per frame
    void update();

    // Checks if a key was pressed this frame
    bool isKeyPressed(int key) const;

    // Checks if a key was released this frame
    bool isKeyReleased(int key) const;

    // Checks if a key is currently held down
    bool isKeyHeld(int key) const;

    // Checks if the quit event was triggered (e.g., by pressing the Escape key)
    bool isQuit() const;

    // Checks if a mouse button was pressed this frame
    bool isMouseButtonPressed(int button) const;

    // Checks if a mouse button was released this frame
    bool isMouseButtonReleased(int button) const;

    // Checks if a mouse button is currently held down
    bool isMouseButtonHeld(int button) const;

    // Retrieves the current mouse position
    void getMousePosition(int &x, int &y) const;

    // Retrieves the mouse movement delta since the last frame
    void getMouseDelta(int &dx, int &dy) const;

private:
    std::unordered_map<int, bool> _keyPressed;
    std::unordered_map<int, bool> _keyReleased;
    std::unordered_map<int, bool> _keyHeld;

    std::unordered_map<int, bool> _mouseButtonPressed;
    std::unordered_map<int, bool> _mouseButtonReleased;
    std::unordered_map<int, bool> _mouseButtonHeld;

    int _mouseX, _mouseY;
    int _prevMouseX, _prevMouseY;
    int _mouseDeltaX, _mouseDeltaY;

};


#endif //CBIT2D_INPUT_H
