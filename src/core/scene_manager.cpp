/**
 * @file    scene_manager.cpp
 * @brief   Source file for the SceneManager class.
 * @details This file contains the implementation of the SceneManager class
 *          which is responsible for registering scenes, activating them, and
 *          dispatching scene lifecycle calls.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-02
 */

#include "cbit/core/scene_manager.hpp"

namespace cbit2d::core {

/**
 * @brief Registers a scene under the provided name.
 * @param sceneName Name used to identify the scene.
 * @param scene Scene instance to store.
 */
void SceneManager::addScene(const std::string& sceneName, const std::shared_ptr<Scene>& scene)
{
    if (scene == nullptr) {
        return;
    }

    _initializedScenes.erase(sceneName);
    _scenes[sceneName] = scene;

    if (_activeSceneName.empty()) {
        _activeSceneName = sceneName;
    }
}

/**
 * @brief Sets the active scene.
 * @param sceneName Name of the scene to activate.
 * @return `true` when the scene exists and becomes active, otherwise `false`.
 */
bool SceneManager::setActiveScene(const std::string& sceneName)
{
    if (!_scenes.contains(sceneName)) {
        return false;
    }

    _activeSceneName = sceneName;
    initializeActiveScene();
    return true;
}

/**
 * @brief Returns the active scene instance.
 * @return Shared pointer to the active scene, or `nullptr` when no active scene exists.
 */
std::shared_ptr<Scene> SceneManager::getActiveScene() const
{
    if (_activeSceneName.empty()) {
        return nullptr;
    }

    const auto iterator = _scenes.find(_activeSceneName);
    if (iterator == _scenes.end()) {
        return nullptr;
    }

    return iterator->second;
}

/**
 * @brief Initializes the active scene once.
 */
void SceneManager::initializeActiveScene()
{
    const auto activeScene = getActiveScene();
    if (activeScene == nullptr) {
        return;
    }

    if (_initializedScenes.contains(_activeSceneName)) {
        return;
    }

    activeScene->initialize();
    _initializedScenes.insert(_activeSceneName);
}

/**
 * @brief Updates the active scene.
 * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
 */
void SceneManager::updateActiveScene(float deltaTimeSeconds)
{
    initializeActiveScene();

    const auto activeScene = getActiveScene();
    if (activeScene == nullptr) {
        return;
    }

    activeScene->update(deltaTimeSeconds);
}

/**
 * @brief Renders the active scene.
 * @param renderer SDL renderer that owns the current frame.
 */
void SceneManager::renderActiveScene(SDL_Renderer* renderer)
{
    initializeActiveScene();

    const auto activeScene = getActiveScene();
    if (activeScene == nullptr) {
        return;
    }

    activeScene->render(renderer);
}

void SceneManager::setActiveSceneUiViewport(const cbit::ui::UiSize viewport)
{
    const auto activeScene = getActiveScene();
    if (activeScene != nullptr) {
        activeScene->setUiViewport(viewport);
    }
}

} // namespace cbit2d::core
