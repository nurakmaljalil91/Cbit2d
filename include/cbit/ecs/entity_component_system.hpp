/**
 * @file    entity_component_system.hpp
 * @brief   Header file for the EntityComponentSystem class.
 * @details This file contains the definition of the ECS world wrapper used by
 *          scenes to create game objects, access entities, and execute custom
 *          systems.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-03
 */

#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include <SDL3/SDL.h>
#include <entt/entity/registry.hpp>

#include "cbit/ecs/components.hpp"
#include "cbit/ecs/drag_system.hpp"
#include "cbit/ecs/game_object.hpp"
#include "cbit/ecs/sprite_animation_system.hpp"
#include "cbit/ecs/sprite_render_system.hpp"
#include "cbit/ecs/ui_system.hpp"

namespace cbit::ecs {

/**
 * @brief Owns a scene-level ECS world.
 * @details This class stores the EnTT registry for one scene, creates game
 *          objects with default components, and runs registered custom systems.
 */
class EntityComponentSystem {
public:
    /**
     * @brief Signature used by custom ECS systems.
     */
    using CustomSystem = std::function<void(EntityComponentSystem&)>;

    /**
     * @brief Creates a new game object in the world.
     * @param tag Optional readable tag assigned to the game object.
     * @return Wrapper for the newly created game object.
     */
    GameObject addGameObject(const std::string& tag = {});

    /**
     * @brief Returns a game object wrapper for an entity handle.
     * @param entity Entity handle to wrap.
     * @return Valid game object when the entity exists, otherwise an invalid object.
     */
    GameObject getGameObject(entt::entity entity);

    /**
     * @brief Returns a game object wrapper from its stable identifier.
     * @param id Stable identifier stored in the world.
     * @return Valid game object when the identifier exists, otherwise an invalid object.
     */
    GameObject getGameObject(GameObjectId id);

    /**
     * @brief Removes the provided game object from the world.
     * @param gameObject Game object to destroy.
     */
    void removeGameObject(const GameObject& gameObject);

    /**
     * @brief Registers a custom system to run during world updates.
     * @param system System function to append to the update list.
     */
    void addSystem(CustomSystem system);

    /**
     * @brief Executes all registered custom systems.
     */
    void update();

    /**
     * @brief Executes all registered custom systems and engine updates.
     * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
     */
    void update(float deltaTimeSeconds);

    /**
     * @brief Renders ECS entities through the default render systems.
     * @param renderer SDL renderer that owns the current frame.
     */
    void render(SDL_Renderer* renderer);

    /**
     * @brief Returns the internal EnTT registry.
     * @return Reference to the registry owned by the world.
     */
    entt::registry& getRegistry();

    /**
     * @brief Returns the internal EnTT registry.
     * @return Const reference to the registry owned by the world.
     */
    const entt::registry& getRegistry() const;

private:
    /**
     * @brief Returns the next stable game object identifier.
     * @return Newly generated identifier.
     */
    GameObjectId generateGameObjectId();

    entt::registry _registry;
    std::unordered_map<GameObjectId, entt::entity> _entitiesById;
    std::vector<CustomSystem> _systems;
    DragSystem _dragSystem;
    SpriteAnimationSystem _spriteAnimationSystem;
    SpriteRenderSystem _spriteRenderSystem;
    UISystem _uiSystem;
    GameObjectId _nextGameObjectId = 1;
};

} // namespace cbit::ecs
