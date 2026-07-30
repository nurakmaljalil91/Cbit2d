/**
 * @file    scene.hpp
 * @brief   Header file for the Scene class.
 * @details This file contains the definition of the Scene base class which is
 *          responsible for providing the gameplay lifecycle interface for
 *          engine-managed scenes.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-02
 */

#pragma once

#include <SDL3/SDL.h>

#include "cbit/ecs/entity_component_system.hpp"
#include "cbit/ui/ui_context.hpp"

namespace cbit2d::core {

/**
 * @brief Base class for gameplay scenes.
 * @details Game-specific scenes should inherit from this class and implement
 *          their startup and per-frame behavior.
 */
class Scene {
public:
    /**
     * @brief Virtual destructor for polymorphic scene cleanup.
     */
    virtual ~Scene() = default;

    /**
     * @brief Initializes the scene before updates begin.
     */
    virtual void initialize() = 0;

    /**
     * @brief Updates the scene once per frame.
     * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
     */
    virtual void update(float deltaTimeSeconds) = 0;

    /**
     * @brief Renders the scene using the provided SDL renderer.
     * @param renderer SDL renderer that owns the current frame.
     */
    virtual void render(SDL_Renderer* renderer);

    /**
     * @brief Returns the scene ECS world.
     * @return Reference to the scene world instance.
     */
    cbit::ecs::EntityComponentSystem& getWorld();

    /**
     * @brief Returns the scene ECS world.
     * @return Const reference to the scene world instance.
     */
    const cbit::ecs::EntityComponentSystem& getWorld() const;

    /** @brief Returns the retained screen UI owned by this scene. */
    cbit::ui::UiContext& getUi();

    /** @brief Updates the logical UI viewport supplied by the application. */
    void setUiViewport(cbit::ui::UiSize viewport);

protected:
    cbit::ecs::EntityComponentSystem world;
    cbit::ui::UiContext _ui;
};

/**
 * @brief Returns the scene ECS world.
 * @return Reference to the scene world instance.
 */
inline cbit::ecs::EntityComponentSystem& Scene::getWorld()
{
    return world;
}

/**
 * @brief Returns the scene ECS world.
 * @return Const reference to the scene world instance.
 */
inline const cbit::ecs::EntityComponentSystem& Scene::getWorld() const
{
    return world;
}

inline cbit::ui::UiContext& Scene::getUi()
{
    return _ui;
}

inline void Scene::setUiViewport(const cbit::ui::UiSize viewport)
{
    _ui.setViewport(viewport);
}

/**
 * @brief Renders the scene using the provided SDL renderer.
 * @param renderer SDL renderer that owns the current frame.
 */
inline void Scene::render(SDL_Renderer* renderer)
{
    world.render(renderer);
    _ui.render(renderer);
}

} // namespace cbit2d::core
