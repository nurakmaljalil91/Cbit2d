/**
 * @file Application.h
 * @brief Header file for the Application class.
 *
 * This file contains the definition of the Application class which encapsulates
 * the SDL2 application setup, main loop, and cleanup logic.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-20
 */
#ifndef CBIT2D_APPLICATION_H
#define CBIT2D_APPLICATION_H


#include <SDL2/SDL.h>
#include "SceneManager.h"

class Application {
public:
    Application();

    Application(const char *windowTitle);

    Application(const char *windowTitle, int windowWidth, int windowHeight);

    ~Application();

    bool init();

    void run();

    void cleanup();

    SceneManager &getSceneManager() {
        return _sceneManager;
    }


private:
    SDL_Window *window;
    const char *_windowTitle;
    int _windowWidth;
    int _windowHeight;
    SDL_Renderer *renderer;
    bool _quit;
    SDL_Event e;

    SceneManager _sceneManager;
};

#endif //CBIT2D_APPLICATION_H
