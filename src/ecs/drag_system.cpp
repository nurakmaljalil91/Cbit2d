/**
 * @file    drag_system.cpp
 * @brief   Source file for the DragSystem class.
 * @details This file contains the implementation of the DragSystem class
 *          which is responsible for mouse-driven dragging of ECS entities.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#include "cbit/ecs/drag_system.hpp"

#include <algorithm>
#include <vector>

#include "cbit/core/input.hpp"
#include "cbit/ecs/components.hpp"
#include "cbit/ecs/entity_component_system.hpp"

namespace cbit::ecs {

namespace {

/**
 * @brief Stores an axis-aligned rectangle for mouse hit testing.
 */
struct Rect {
    float left = 0.0F;
    float top = 0.0F;
    float right = 0.0F;
    float bottom = 0.0F;
};

/**
 * @brief Builds a draggable hit rectangle for an entity.
 * @param transform Transform used to position the entity.
 * @param collider Optional collider used as the preferred hit area.
 * @param sprite Optional sprite used as the fallback hit area.
 * @return Hit rectangle in world space.
 */
Rect makeRect(
    const TransformComponent& transform,
    const ColliderComponent* collider,
    const SpriteComponent* sprite
)
{
    glm::vec2 size {32.0F, 32.0F};
    glm::vec2 offset {0.0F, 0.0F};

    if (collider != nullptr) {
        size = collider->size;
        offset = collider->offset;
    } else if (sprite != nullptr) {
        size = sprite->size;
    }

    const glm::vec2 center = transform.position + offset;
    const glm::vec2 halfSize = size * 0.5F;
    return {
        center.x - halfSize.x,
        center.y - halfSize.y,
        center.x + halfSize.x,
        center.y + halfSize.y
    };
}

/**
 * @brief Returns whether a point lies inside an axis-aligned rectangle.
 * @param rect Rectangle to test.
 * @param point Point to test.
 * @return `true` when the point is inside the rectangle.
 */
bool contains(const Rect& rect, const SDL_FPoint& point)
{
    return point.x >= rect.left
        && point.x <= rect.right
        && point.y >= rect.top
        && point.y <= rect.bottom;
}

/**
 * @brief Returns the sprite render order used for topmost drag selection.
 * @param registry Registry storing the entity components.
 * @param entity Entity to query.
 * @return Sprite render order when present, otherwise zero.
 */
int getRenderOrder(entt::registry& registry, const entt::entity entity)
{
    if (registry.all_of<SpriteComponent>(entity)) {
        return registry.get<SpriteComponent>(entity).renderOrder;
    }

    return 0;
}

} // namespace

/**
 * @brief Updates mouse-driven dragging state for one frame.
 * @param ecs ECS world that owns the draggable entities.
 */
void DragSystem::update(EntityComponentSystem& ecs)
{
    auto& registry = ecs.getRegistry();
    const SDL_FPoint mousePosition = cbit2d::core::Input::getMousePosition();

    if (_activeEntity != entt::null) {
        if (!registry.valid(_activeEntity)
            || !registry.all_of<TransformComponent, DragableComponent>(_activeEntity)) {
            _activeEntity = entt::null;
        }
    }

    if (_activeEntity != entt::null) {
        auto& dragable = registry.get<DragableComponent>(_activeEntity);
        if (!dragable.enabled || !cbit2d::core::Input::isMouseButtonHeld(cbit2d::core::MouseButton::Left)) {
            dragable.isDragging = false;
            _activeEntity = entt::null;
        } else {
            auto& transform = registry.get<TransformComponent>(_activeEntity);
            transform.position = {
                mousePosition.x - dragable.grabOffset.x,
                mousePosition.y - dragable.grabOffset.y
            };
            return;
        }
    }

    if (!cbit2d::core::Input::isMouseButtonPressed(cbit2d::core::MouseButton::Left)) {
        return;
    }

    auto view = registry.view<TransformComponent, DragableComponent>();
    std::vector<entt::entity> candidates;
    candidates.reserve(view.size_hint());

    for (const auto entity : view) {
        candidates.push_back(entity);
    }

    std::stable_sort(
        candidates.begin(),
        candidates.end(),
        [&registry](const entt::entity left, const entt::entity right) {
            return getRenderOrder(registry, left) > getRenderOrder(registry, right);
        }
    );

    for (const auto entity : candidates) {
        auto& dragable = registry.get<DragableComponent>(entity);
        if (!dragable.enabled) {
            continue;
        }

        const auto& transform = registry.get<TransformComponent>(entity);
        const ColliderComponent* collider = registry.try_get<ColliderComponent>(entity);
        const SpriteComponent* sprite = registry.try_get<SpriteComponent>(entity);
        const Rect rect = makeRect(transform, collider, sprite);
        if (!contains(rect, mousePosition)) {
            continue;
        }

        dragable.isDragging = true;
        dragable.grabOffset = {
            mousePosition.x - transform.position.x,
            mousePosition.y - transform.position.y
        };
        _activeEntity = entity;
        return;
    }
}

} // namespace cbit::ecs
