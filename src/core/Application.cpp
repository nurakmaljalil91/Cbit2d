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
        window(nullptr),
        renderer(nullptr),
        _quit(false) {
    _windowTitle = "CBit 2D Application";
    _windowWidth = 640;
    _windowHeight = 480;
}

Application::Application(const char *windowTitle) :
        window(nullptr),
        renderer(nullptr),
        _quit(false) {
    _windowTitle = windowTitle;
    _windowWidth = 640;
    _windowHeight = 480;
}

Application::Application(const char *windowTitle, int windowWidth, int windowHeight) :
        window(nullptr),
        renderer(nullptr),
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

    window = SDL_CreateWindow(_windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowWidth,
                              _windowHeight,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        LOG_ERROR("Window could not be created! SDL_Error: {}", SDL_GetError());
        return false;
    }
    LOG_INFO("Window created successfully");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        LOG_ERROR("Renderer could not be created! SDL_Error: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        return false;
    }

    return true;
}

void Application::run() {
    while (!_quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                _quit = true;
            }
        }

        _sceneManager.update();

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        _sceneManager.render();

        SDL_RenderPresent(renderer);
    }
}

void Application::cleanup() {
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
    LOG_INFO("SDL _quit");
}
