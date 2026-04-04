/**
 * @file    game_object.hpp
 * @brief   Header file for the GameObject class.
 * @details This file contains the definition of the GameObject wrapper which
 *          provides component management on top of an EnTT entity handle.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-03
 */

#pragma once

#include <cassert>
#include <utility>

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

namespace cbit::ecs {

/**
 * @brief Lightweight wrapper around an EnTT entity.
 * @details This class exposes typed helpers for adding, querying, retrieving,
 *          and removing components from a game object in an ECS world.
 */
class GameObject {
public:
    /**
     * @brief Constructs an invalid game object.
     */
    GameObject() = default;

    /**
     * @brief Constructs a game object from an entity handle and registry.
     * @param entity Entity handle owned by the ECS world.
     * @param registry Registry that stores the entity components.
     */
    GameObject(entt::entity entity, entt::registry* registry) : _entity(entity), _registry(registry)
    {
    }

    /**
     * @brief Adds a component to the game object.
     * @tparam TComponent Component type to add.
     * @tparam TArgs Constructor argument types.
     * @param args Arguments forwarded to the component constructor.
     * @return Reference to the newly created component.
     */
    template <typename TComponent, typename... TArgs>
    TComponent& addComponent(TArgs&&... args)
    {
        assert(isValid());
        return _registry->emplace<TComponent>(_entity, std::forward<TArgs>(args)...);
    }

    /**
     * @brief Returns the requested component from the game object.
     * @tparam TComponent Component type to retrieve.
     * @return Reference to the stored component.
     */
    template <typename TComponent>
    TComponent& getComponent()
    {
        assert(isValid());
        return _registry->get<TComponent>(_entity);
    }

    /**
     * @brief Returns the requested component from the game object.
     * @tparam TComponent Component type to retrieve.
     * @return Const reference to the stored component.
     */
    template <typename TComponent>
    const TComponent& getComponent() const
    {
        assert(isValid());
        return _registry->get<TComponent>(_entity);
    }

    /**
     * @brief Removes a component from the game object.
     * @tparam TComponent Component type to remove.
     */
    template <typename TComponent>
    void removeComponent()
    {
        assert(isValid());
        _registry->remove<TComponent>(_entity);
    }

    /**
     * @brief Checks whether the game object has the requested component.
     * @tparam TComponent Component type to test.
     * @return `true` when the component exists on the game object.
     */
    template <typename TComponent>
    bool hasComponent() const
    {
        return isValid() && _registry->all_of<TComponent>(_entity);
    }

    /**
     * @brief Returns whether the game object references a live entity.
     * @return `true` when the entity handle and registry are valid.
     */
    bool isValid() const;

    /**
     * @brief Returns the raw EnTT entity handle.
     * @return Stored entity handle.
     */
    entt::entity getHandle() const;

    /**
     * @brief Converts the game object to a boolean validity flag.
     * @return `true` when the game object is valid.
     */
    explicit operator bool() const;

private:
    entt::entity _entity = entt::null;
    entt::registry* _registry = nullptr;
};

} // namespace cbit::ecs
