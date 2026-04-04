/**
 * @file    application.hpp
 * @brief   Header file for the Application class.
 * @details This file contains the definition of the Application class which is
 *          responsible for creating the SDL window, owning the renderer,
 *          driving the program loop, and exposing the scene manager.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-02
 */

#pragma once

#include <SDL3/SDL.h>

#include "cbit/core/scene_manager.hpp"

namespace cbit2d::core {

/**
 * @brief Owns the main SDL application state.
 * @details This class centralizes window creation, event handling, frame
 *          iteration, shutdown, and scene manager access for the bootstrap
 *          application.
 */
class Application {
public:
    /**
     * @brief Returns the application scene manager.
     * @return Reference to the internal scene manager.
     */
    SceneManager& getSceneManager();

    /**
     * @brief Runs the application from startup to shutdown.
     * @param title Window title.
     * @param width Window width in pixels.
     * @param height Window height in pixels.
     * @return Exit code for the process.
     */
    int run(const char* title, int width, int height);

private:
    /**
     * @brief Initializes SDL and creates the main window.
     * @param title Window title.
     * @param width Window width in pixels.
     * @param height Window height in pixels.
     * @return `true` when initialization succeeds, otherwise `false`.
     */
    bool initialize(const char* title, int width, int height);

    /**
     * @brief Processes one SDL event.
     * @param event Event to handle.
     * @return `true` when the application should keep running, otherwise `false`.
     */
    bool handleEvent(const SDL_Event& event);

    /**
     * @brief Renders one frame of the application.
     * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
     */
    void iterate(float deltaTimeSeconds);

    /**
     * @brief Toggles the window between windowed and fullscreen modes.
     */
    void toggleFullscreen();

    /**
     * @brief Releases owned SDL resources and shuts SDL down.
     */
    void shutdown();

    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    bool _isFullscreen = false;
    SceneManager _sceneManager;
};

} // namespace cbit2d::core
