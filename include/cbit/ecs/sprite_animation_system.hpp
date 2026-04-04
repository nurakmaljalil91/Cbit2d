/**
 * @file    sprite_animation_system.hpp
 * @brief   Header file for the SpriteAnimationSystem class.
 * @details This file contains the definition of the SpriteAnimationSystem
 *          class which is responsible for advancing sprite animation playback
 *          and applying the current animation frame to sprite components.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#pragma once

namespace cbit::ecs {

class EntityComponentSystem;

/**
 * @brief Updates sprite animation playback for ECS entities.
 */
class SpriteAnimationSystem {
public:
    /**
     * @brief Advances animated sprites for one frame.
     * @param ecs ECS world that owns the entities and components.
     * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
     */
    void update(EntityComponentSystem& ecs, float deltaTimeSeconds);
};

} // namespace cbit::ecs
