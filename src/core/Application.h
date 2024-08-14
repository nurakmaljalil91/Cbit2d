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

#ifdef ENABLE_EDITOR
#include "../editor/Editor.h"
#endif

struct WindowConfig {
    const char *title;
    int width;
    int height;
    bool isFullscreen = false;
};

class Application {
public:
    Application(const char *windowTitle, int windowWidth, int windowHeight, bool isFullscreen);

    ~Application();

    bool init();

    void run();

    void cleanup();

    SceneManager &getSceneManager();

private:
    WindowConfig _windowConfig;
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    bool _isQuit;
    Input _input;

    // fonts
    TTF_Font *_defaultFont;
    Uint32 _lastFrameTick{};

    // fullscreen
    void toggleFullscreen();

    // FPS counter
    Uint32 _frameCount = 0;
    Uint32 _lastFPSTime = 0;
    Uint32 _fps = 0;

    // manager
    SceneManager _sceneManager;

    // editor
#ifdef ENABLE_EDITOR
    Editor _editor;
#endif
    static void
    renderApplicationTexts(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y, SDL_Color color);
};

#endif //CBIT2D_APPLICATION_H
