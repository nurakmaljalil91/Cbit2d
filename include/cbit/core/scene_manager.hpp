/**
 * @file    scene_manager.hpp
 * @brief   Header file for the SceneManager class.
 * @details This file contains the definition of the SceneManager class which
 *          is responsible for registering scenes, selecting the active scene,
 *          and driving scene lifecycle calls.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-02
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <SDL3/SDL.h>

#include "cbit/core/scene.hpp"

namespace cbit2d::core {

/**
 * @brief Owns and updates registered gameplay scenes.
 * @details The scene manager stores scene instances by name, keeps track of
 *          the active scene, and ensures each scene is initialized once before
 *          its first update.
 */
class SceneManager {
public:
    /**
     * @brief Registers a scene instance under a string key.
     * @param sceneName Name used to access the scene later.
     * @param scene Scene instance to register.
     */
    void addScene(const std::string& sceneName, const std::shared_ptr<Scene>& scene);

    /**
     * @brief Sets the currently active scene.
     * @param sceneName Name of the scene to activate.
     * @return `true` when the scene exists and becomes active, otherwise `false`.
     */
    bool setActiveScene(const std::string& sceneName);

    /**
     * @brief Returns the currently active scene.
     * @return Shared pointer to the active scene, or `nullptr` when no scene is active.
     */
    std::shared_ptr<Scene> getActiveScene() const;

    /**
     * @brief Initializes the active scene if needed.
     */
    void initializeActiveScene();

    /**
     * @brief Updates the active scene once per frame.
     * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
     */
    void updateActiveScene(float deltaTimeSeconds);

    /**
     * @brief Renders the active scene.
     * @param renderer SDL renderer that owns the current frame.
     */
    void renderActiveScene(SDL_Renderer* renderer);

    /** @brief Updates the active scene's logical screen UI viewport. */
    void setActiveSceneUiViewport(cbit::ui::UiSize viewport);

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;
    std::unordered_set<std::string> _initializedScenes;
    std::string _activeSceneName;
};

} // namespace cbit2d::core
