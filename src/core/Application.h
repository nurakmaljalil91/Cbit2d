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
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "SceneManager.h"
#include "Input.h"

class Application {
public:
    Application(const char *windowTitle, int windowWidth, int windowHeight, bool isFullscreen);

    ~Application();

    bool init();

    void run();

    void cleanup();

    SceneManager &getSceneManager();

private:
    SDL_Window *_window;
    const char *_windowTitle;
    int _windowWidth;
    int _windowHeight;
    SDL_Renderer *_renderer;
    bool _isQuit;
    bool _isFullscreen = false;
    bool _isAllowToggleFullscreen = false;
    SDL_Event _event{};
    Input _input;
    TTF_Font* _defaultFont;

    // fullscreen
    void toggleFullscreen();

    // FPS counter
    Uint32 _frameCount = 0;
    Uint32 _lastFPSTime = 0;
    Uint32 _fps = 0;

    // manager
    SceneManager _sceneManager;

    void _renderApplicationTexts(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y, SDL_Color color);
};

#endif //CBIT2D_APPLICATION_H
