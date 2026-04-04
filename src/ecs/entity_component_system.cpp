/**
 * @file    entity_component_system.cpp
 * @brief   Source file for the EntityComponentSystem class.
 * @details This file contains the implementation of the scene ECS world which
 *          creates game objects, tracks identifiers, and runs custom systems.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-03
 */

#include "cbit/ecs/entity_component_system.hpp"

namespace cbit::ecs {

/**
 * @brief Creates a new game object in the world.
 * @param tag Optional readable tag assigned to the game object.
 * @return Wrapper for the newly created game object.
 */
GameObject EntityComponentSystem::addGameObject(const std::string& tag)
{
    const entt::entity entity = _registry.create();
    GameObject gameObject(entity, &_registry);
    const GameObjectId id = generateGameObjectId();

    gameObject.addComponent<IdComponent>(id);
    gameObject.addComponent<TagComponent>(tag);
    gameObject.addComponent<TransformComponent>();

    _entitiesById[id] = entity;
    return gameObject;
}

/**
 * @brief Returns a game object wrapper for an entity handle.
 * @param entity Entity handle to wrap.
 * @return Valid game object when the entity exists, otherwise an invalid object.
 */
GameObject EntityComponentSystem::getGameObject(entt::entity entity)
{
    if (!_registry.valid(entity)) {
        return {};
    }

    return {entity, &_registry};
}

/**
 * @brief Returns a game object wrapper from its stable identifier.
 * @param id Stable identifier stored in the world.
 * @return Valid game object when the identifier exists, otherwise an invalid object.
 */
GameObject EntityComponentSystem::getGameObject(GameObjectId id)
{
    const auto iterator = _entitiesById.find(id);
    if (iterator == _entitiesById.end()) {
        return {};
    }

    return getGameObject(iterator->second);
}

/**
 * @brief Removes the provided game object from the world.
 * @param gameObject Game object to destroy.
 */
void EntityComponentSystem::removeGameObject(const GameObject& gameObject)
{
    if (!gameObject.isValid()) {
        return;
    }

    const auto& idComponent = gameObject.getComponent<IdComponent>();
    _entitiesById.erase(idComponent.id);
    _registry.destroy(gameObject.getHandle());
}

/**
 * @brief Registers a custom system to run during world updates.
 * @param system System function to append to the update list.
 */
void EntityComponentSystem::addSystem(CustomSystem system)
{
    _systems.push_back(std::move(system));
}

/**
 * @brief Executes all registered custom systems.
 */
void EntityComponentSystem::update()
{
    update(0.0F);
}

/**
 * @brief Executes all registered custom systems and engine updates.
 * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
 */
void EntityComponentSystem::update(float deltaTimeSeconds)
{
    _uiSystem.update(*this);
    _dragSystem.update(*this);

    for (auto& system : _systems) {
        system(*this);
    }

    _spriteAnimationSystem.update(*this, deltaTimeSeconds);
}

/**
 * @brief Renders ECS entities through the default render systems.
 * @param renderer SDL renderer that owns the current frame.
 */
void EntityComponentSystem::render(SDL_Renderer* renderer)
{
    _spriteRenderSystem.render(*this, renderer);
    _uiSystem.render(*this, renderer);
}

/**
 * @brief Returns the internal EnTT registry.
 * @return Reference to the registry owned by the world.
 */
entt::registry& EntityComponentSystem::getRegistry()
{
    return _registry;
}

/**
 * @brief Returns the internal EnTT registry.
 * @return Const reference to the registry owned by the world.
 */
const entt::registry& EntityComponentSystem::getRegistry() const
{
    return _registry;
}

/**
 * @brief Returns the next stable game object identifier.
 * @return Newly generated identifier.
 */
GameObjectId EntityComponentSystem::generateGameObjectId()
{
    return _nextGameObjectId++;
}

} // namespace cbit::ecs
