/**
 * @file    application.cpp
 * @brief   Source file for the Application class.
 * @details This file contains the implementation of the Application class
 *          which is responsible for creating the SDL window, handling the main
 *          loop, updating the active scene, and releasing SDL resources.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-02
 */

#include "cbit/core/application.hpp"

#include "cbit/core/input.hpp"
#include "cbit/core/logger.hpp"

namespace cbit2d::core {

/**
 * @brief Returns the application scene manager.
 * @return Reference to the internal scene manager.
 */
SceneManager& Application::getSceneManager()
{
    return _sceneManager;
}

/**
 * @brief Runs the application lifecycle.
 * @param title Window title.
 * @param width Window width in pixels.
 * @param height Window height in pixels.
 * @return Exit code for the process.
 */
int Application::run(const char* title, int width, int height)
{
    if (!initialize(title, width, height)) {
        shutdown();
        return 1;
    }

    _sceneManager.initializeActiveScene();

    bool isRunning = true;
    Uint64 previousFrameCounter = SDL_GetPerformanceCounter();

    while (isRunning) {
        Input::beginFrame();
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            isRunning = handleEvent(event);
            if (!isRunning) {
                break;
            }
        }

        if (isRunning) {
            const Uint64 currentFrameCounter = SDL_GetPerformanceCounter();
            const Uint64 elapsedCounter = currentFrameCounter - previousFrameCounter;
            previousFrameCounter = currentFrameCounter;

            const double performanceFrequency = static_cast<double>(SDL_GetPerformanceFrequency());
            const float deltaTimeSeconds = performanceFrequency > 0.0
                ? static_cast<float>(static_cast<double>(elapsedCounter) / performanceFrequency)
                : 0.0F;

            iterate(deltaTimeSeconds);
        }
    }

    shutdown();
    return 0;
}

/**
 * @brief Initializes SDL and creates the SDL window and renderer.
 * @param title Window title.
 * @param width Window width in pixels.
 * @param height Window height in pixels.
 * @return `true` when the SDL window and renderer are created successfully,
 *         otherwise `false`.
 */
bool Application::initialize(const char* title, int width, int height)
{
    Logger::initialize();
    Logger::info("CBit 2D Application started");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        Logger::error("Couldn't initialize SDL: {}", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(title, width, height, 0, &_window, &_renderer)) {
        Logger::error("Couldn't create window and renderer: {}", SDL_GetError());
        return false;
    }

    _isFullscreen = false;
    return true;
}

/**
 * @brief Processes SDL events for the application.
 * @param event Event to handle.
 * @return `true` when the application should keep running, otherwise `false`.
 */
bool Application::handleEvent(const SDL_Event& event)
{
    Input::processEvent(event);

    if (event.type == SDL_EVENT_QUIT) {
        return false;
    }

    if (Input::isKeyPressed(Key::F11)) {
        toggleFullscreen();
        return true;
    }

    if (Input::isKeyPressed(Key::Escape)) {
        return false;
    }

    return true;
}

/**
 * @brief Renders one frame and updates the active scene.
 * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
 */
void Application::iterate(float deltaTimeSeconds)
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);

    _sceneManager.updateActiveScene(deltaTimeSeconds);
    _sceneManager.renderActiveScene(_renderer);

    SDL_RenderPresent(_renderer);
}

/**
 * @brief Toggles the window between windowed and fullscreen modes.
 */
void Application::toggleFullscreen()
{
    _isFullscreen = !_isFullscreen;

    if (!SDL_SetWindowFullscreen(_window, _isFullscreen)) {
        Logger::error("Couldn't toggle fullscreen mode: {}", SDL_GetError());
        _isFullscreen = !_isFullscreen;
    }
}

/**
 * @brief Releases the SDL renderer and window and shuts SDL down.
 */
void Application::shutdown()
{
    if (_renderer != nullptr) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    SDL_Quit();
}

} // namespace cbit2d::core
