/**
 * @file Application.cpp
 * @brief Implementation file for the Application class.
 *
 * This file contains the implementation of the Application class which encapsulates
 * the SDL2 application setup, main loop, and cleanup logic.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-20
 */
#include <iostream>
#include "Application.h"
#include "../utilities/LocalMachine.h"
#include "AssetManager.h"

Application::Application(const char *windowTitle, int windowWidth, int windowHeight, bool isFullscreen) :
        _windowTitle(windowTitle),
        _windowWidth(windowWidth),
        _windowHeight(windowHeight),
        _window(nullptr),
        _renderer(nullptr),
        _isQuit(false),
        _defaultFont(nullptr),
        _isFullscreen(isFullscreen) {}


Application::~Application() {
    cleanup();
}

bool Application::init() {
    Logger::init();


    LOG_INFO("Starting Cbit 2D application");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        LOG_ERROR("SDL could not initialize! SDL_Error: {}", SDL_GetError());
        return false;
    }

    LOG_INFO("SDL initialized successfully");

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        LOG_ERROR("SDL_mixer could not initialize! SDL_mixer Error: {}", Mix_GetError());
        return false;
    }

    if (_isFullscreen) {
        _window = SDL_CreateWindow(_windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   _windowWidth, _windowHeight,
                                   SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    } else {
        _window = SDL_CreateWindow(_windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   _windowWidth, _windowHeight,
                                   SDL_WINDOW_SHOWN);
    }

    if (_window == nullptr) {
        LOG_ERROR("Window could not be created! SDL_Error: {}", SDL_GetError());
        return false;
    }

    LOG_INFO("Window created successfully");

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (_renderer == nullptr) {
        LOG_ERROR("Renderer could not be created! SDL_Error: {}", SDL_GetError());
        SDL_DestroyWindow(_window);
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        LOG_ERROR("SDL_image could not initialize! SDL_image Error: {}", IMG_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        LOG_ERROR("SDL_ttf could not initialize! TTF_Error: {}", TTF_GetError());
        return false;
    }

    _defaultFont = TTF_OpenFont(LocalMachine::getFontPath(), 24);
    if (!_defaultFont) {
        LOG_ERROR("Failed to load font: %s", TTF_GetError());
    }

    AssetManager::getInstance().init(_renderer);

    return true;
}

void Application::run() {
    _lastFPSTime = SDL_GetTicks(); // Initialize the last FPS update time
    float deltaTime = 0.0f;
    _lastFrameTick = SDL_GetTicks();

    while (!_isQuit) {
        Uint32 startTick = SDL_GetTicks();

        _input.update();

        if (_input.isQuit() || _input.isKeyPressed(SDLK_ESCAPE)) {
            _isQuit = true;
        }

        if (_input.isKeyPressed(SDLK_F11)) {
            toggleFullscreen();
        }

        // Calculate delta time
        Uint32 currentTick = SDL_GetTicks();
        deltaTime = static_cast<float>(currentTick - _lastFrameTick) / 1000.0f;
        _lastFrameTick = currentTick;

        // Calculate FPS
        _frameCount++;
        if (currentTick - _lastFPSTime >= 1000) { // Update FPS every second
            _fps = _frameCount;
            _frameCount = 0;
            _lastFPSTime = currentTick;
        }

        _sceneManager.update(deltaTime, _input);

        SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(_renderer);

        _sceneManager.render(_renderer);

        // Display FPS
        std::string fpsText = "FPS: " + std::to_string(_fps);
        _renderApplicationTexts(_renderer, fpsText.c_str(), _defaultFont, 5, 5, SDL_Color{255, 255, 255, 255});

        SDL_RenderPresent(_renderer);

        // Cap frame rate
        Uint32 endTick = SDL_GetTicks();
        Uint32 frameTime = endTick - startTick;
        if (frameTime < 1000 / 60) { // Assuming a 60 FPS cap
            SDL_Delay((1000 / 60) - frameTime);
        }
    }
}

void Application::cleanup() {
    _sceneManager.cleanup();
    AssetManager::getInstance().cleanup();

    if (_renderer != nullptr) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
    Mix_CloseAudio();
    TTF_CloseFont(_defaultFont);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    LOG_INFO("SDL _quit");
}

void Application::_renderApplicationTexts(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y,
                                          SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (surface == nullptr) {
        LOG_ERROR("Unable to render text surface! TTF_Error: {}", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        LOG_ERROR("Unable to create texture from rendered text! SDL_Error: {}", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

SceneManager &Application::getSceneManager() {
    return _sceneManager;
}

void Application::toggleFullscreen() {
    Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool isFullscreen = SDL_GetWindowFlags(_window) & fullscreenFlag;
    SDL_SetWindowFullscreen(_window, isFullscreen ? 0 : fullscreenFlag);
//    SDL_ShowCursor(isFullscreen);
}

