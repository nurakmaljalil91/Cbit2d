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
    void update();

    bool isKeyPressed(int key);

    bool isKeyReleased(int key);

    bool isKeyHeld(int key);

    bool isQuit();

    bool isMouseButtonPressed(int button);

    bool isMouseButtonReleased(Uint8 button);

    bool isMouseButtonHeld(Uint8 button);

    void getMousePosition(int &x, int &y);

    void getMouseDelta(int &dx, int &dy);

private:
    std::unordered_map<int, bool> _keyPressed;
    std::unordered_map<int, bool> _keyReleased;
    std::unordered_map<int, bool> _keyHeld;

    std::unordered_map<Uint8, bool> _mouseButtonPressed;
    std::unordered_map<Uint8, bool> _mouseButtonReleased;
    std::unordered_map<Uint8, bool> _mouseButtonHeld;

    int _mouseX, _mouseY;
    int _prevMouseX, _prevMouseY;
    int _mouseDeltaX, _mouseDeltaY;

};


#endif //CBIT2D_INPUT_H
