/**
 * @file    components.hpp
 * @brief   Header file for default ECS components.
 * @details This file contains the definition of core engine components used by
 *          game objects, including transform, tag, identifier, sprite,
 *          animation, collision, and UI data.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-03
 */

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <glm/vec2.hpp>

namespace cbit::ecs {

/**
 * @brief Type used to identify game objects across the ECS world.
 */
using GameObjectId = std::uint64_t;

/**
 * @brief Stores the stable identifier of a game object.
 */
struct IdComponent {
    GameObjectId id = 0;
};

/**
 * @brief Stores a readable label for a game object.
 */
struct TagComponent {
    std::string tag;
};

/**
 * @brief Stores 2D transform data for a game object.
 */
struct TransformComponent {
    glm::vec2 position {0.0F, 0.0F};
    glm::vec2 scale {1.0F, 1.0F};
    float rotation = 0.0F;
};

/**
 * @brief Stores sprite rendering data for a game object.
 */
struct SpriteComponent {
    std::string assetPath;
    glm::vec2 textureSize {0.0F, 0.0F};
    glm::vec2 sourcePosition {0.0F, 0.0F};
    glm::vec2 sourceSize {32.0F, 32.0F};
    glm::vec2 size {32.0F, 32.0F};
    int renderOrder = 0;
};

/**
 * @brief Stores the frame range for one sprite animation state.
 */
struct SpriteAnimationClip {
    glm::vec2 startSourcePosition {0.0F, 0.0F};
    int frameCount = 1;
    float frameDurationSeconds = 0.15F;
};

/**
 * @brief Stores frame-based sprite animation data for a game object.
 * @details Animation clip names are game-defined so gameplay code can choose
 *          state labels such as `idle`, `walk`, `run`, or anything else
 *          without hardcoding those concepts into the engine.
 */
struct SpriteAnimationComponent {
    std::unordered_map<std::string, SpriteAnimationClip> clips;
    std::string currentClipName;
    std::string previousClipName;
    float elapsedSeconds = 0.0F;
    int currentFrameIndex = 0;
};

/**
 * @brief Stores axis-aligned collider data for a game object.
 * @details The collider size is expressed in local space and centered on the
 *          game object's transform position by default.
 */
struct ColliderComponent {
    glm::vec2 size {32.0F, 32.0F};
    glm::vec2 offset {0.0F, 0.0F};
    bool isTrigger = false;
    bool isStatic = false;
};

/**
 * @brief Stores draggable interaction state for a game object.
 */
struct DragableComponent {
    bool enabled = true;
    bool isDragging = false;
    glm::vec2 grabOffset {0.0F, 0.0F};
};

/**
 * @brief Stores UI text rendering data for a game object.
 */
struct TextComponent {
    std::string content;
    std::string fontPath;
    float fontSize = 24.0F;
    SDL_Color color {255, 255, 255, 255};
    int wrapWidth = 0;
    bool centered = true;
};

/**
 * @brief Stores UI button rendering and click behavior data.
 */
struct ButtonComponent {
    glm::vec2 size {200.0F, 64.0F};
    SDL_Color backgroundColor {35, 46, 77, 255};
    SDL_Color hoverColor {54, 71, 118, 255};
    SDL_Color pressedColor {22, 29, 49, 255};
    SDL_Color borderColor {241, 196, 15, 255};
    std::function<void()> onClick;
};

} // namespace cbit::ecs
