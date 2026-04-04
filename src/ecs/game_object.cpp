/**
 * @file    game_object.cpp
 * @brief   Source file for the GameObject class.
 * @details This file contains the implementation of the GameObject wrapper
 *          which manages access to components stored in an ECS registry.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-03
 */

#include "cbit/ecs/game_object.hpp"

namespace cbit::ecs {

/**
 * @brief Returns whether the game object references a live entity.
 * @return `true` when the entity handle and registry are valid.
 */
bool GameObject::isValid() const
{
    return _registry != nullptr && _entity != entt::null && _registry->valid(_entity);
}

/**
 * @brief Returns the raw EnTT entity handle.
 * @return Stored entity handle.
 */
entt::entity GameObject::getHandle() const
{
    return _entity;
}

/**
 * @brief Converts the game object to a boolean validity flag.
 * @return `true` when the game object is valid.
 */
GameObject::operator bool() const
{
    return isValid();
}

} // namespace cbit::ecs
