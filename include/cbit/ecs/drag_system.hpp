/**
 * @file    drag_system.hpp
 * @brief   Header file for the DragSystem class.
 * @details This file contains the definition of the DragSystem class which is
 *          responsible for mouse-driven dragging of ECS entities tagged with
 *          DragableComponent.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#pragma once

#include <entt/entity/entity.hpp>

namespace cbit::ecs {

class EntityComponentSystem;

/**
 * @brief Updates drag interactions for draggable ECS entities.
 */
class DragSystem {
public:
    /**
     * @brief Updates mouse-driven dragging state for one frame.
     * @param ecs ECS world that owns the draggable entities.
     */
    void update(EntityComponentSystem& ecs);

private:
    entt::entity _activeEntity = entt::null;
};

} // namespace cbit::ecs
