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
#include <utility>
#include "Application.h"
#include "../utilities/Logger.h"

Application::Application() :
        _window(nullptr),
        _renderer(nullptr),
        _quit(false) {
    _windowTitle = "CBit 2D Application";
    _windowWidth = 640;
    _windowHeight = 480;
}

Application::Application(const char *windowTitle) :
        _window(nullptr),
        _renderer(nullptr),
        _quit(false) {
    _windowTitle = windowTitle;
    _windowWidth = 640;
    _windowHeight = 480;
}

Application::Application(const char *windowTitle, int windowWidth, int windowHeight) :
        _window(nullptr),
        _renderer(nullptr),
        _quit(false) {
    _windowTitle = windowTitle;
    _windowWidth = windowWidth;
    _windowHeight = windowHeight;
}


Application::~Application() {
    cleanup();
}

bool Application::init() {
    Logger::init();
    LOG_INFO("Starting SDL2 application");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_ERROR("SDL could not initialize! SDL_Error: {}", SDL_GetError());
        return false;
    }
    LOG_INFO("SDL initialized successfully");

    _window = SDL_CreateWindow(_windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowWidth,
                               _windowHeight,
                               SDL_WINDOW_SHOWN);
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

    _sceneManager.setup(_renderer);

    return true;
}

void Application::run() {
    while (!_quit) {
        while (SDL_PollEvent(&_event) != 0) {
            _sceneManager.handleInput(_event);
            if (_event.type == SDL_QUIT) {
                _quit = true;
            }
        }

        _sceneManager.update();

        SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(_renderer);

        _sceneManager.render(_renderer);

        SDL_RenderPresent(_renderer);
    }
}

void Application::cleanup() {
    _sceneManager.cleanup();

    if (_renderer != nullptr) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
    SDL_Quit();
    LOG_INFO("SDL _quit");
}
