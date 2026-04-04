/**
 * @file    sprite_animation_system.cpp
 * @brief   Source file for the SpriteAnimationSystem class.
 * @details This file contains the implementation of the SpriteAnimationSystem
 *          class which advances sprite animation clips and updates sprite
 *          source positions for rendering.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#include "cbit/ecs/sprite_animation_system.hpp"

#include "cbit/ecs/components.hpp"
#include "cbit/ecs/entity_component_system.hpp"

namespace cbit::ecs {

/**
 * @brief Advances animated sprites for one frame.
 * @param ecs ECS world that owns the entities and components.
 * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
 */
void SpriteAnimationSystem::update(EntityComponentSystem& ecs, float deltaTimeSeconds)
{
    auto view = ecs.getRegistry().view<SpriteComponent, SpriteAnimationComponent>();
    for (const auto entity : view) {
        auto& sprite = view.get<SpriteComponent>(entity);
        auto& animation = view.get<SpriteAnimationComponent>(entity);
        if (animation.currentClipName.empty()) {
            continue;
        }

        const auto clipIterator = animation.clips.find(animation.currentClipName);
        if (clipIterator == animation.clips.end()) {
            continue;
        }

        const auto& clip = clipIterator->second;
        if (clip.frameCount <= 0) {
            continue;
        }

        if (animation.previousClipName != animation.currentClipName) {
            animation.previousClipName = animation.currentClipName;
            animation.elapsedSeconds = 0.0F;
            animation.currentFrameIndex = 0;
        }

        animation.elapsedSeconds += deltaTimeSeconds;
        while (clip.frameDurationSeconds > 0.0F && animation.elapsedSeconds >= clip.frameDurationSeconds) {
            animation.elapsedSeconds -= clip.frameDurationSeconds;
            animation.currentFrameIndex = (animation.currentFrameIndex + 1) % clip.frameCount;
        }

        sprite.sourcePosition = clip.startSourcePosition;
        sprite.sourcePosition.x += sprite.sourceSize.x * static_cast<float>(animation.currentFrameIndex);
    }
}

} // namespace cbit::ecs
