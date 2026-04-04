/**
 * @file    sprite_render_system.cpp
 * @brief   Source file for the SpriteRenderSystem class.
 * @details This file contains the implementation of the SpriteRenderSystem
 *          class which is responsible for loading sprite textures and
 *          rendering ECS sprite entities through SDL.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#include "cbit/ecs/sprite_render_system.hpp"

#include <algorithm>
#include <filesystem>
#include <vector>

#include <SDL3_image/SDL_image.h>

#include "cbit/core/logger.hpp"
#include "cbit/ecs/components.hpp"
#include "cbit/ecs/entity_component_system.hpp"

namespace cbit::ecs {

/**
 * @brief Releases any cached SDL textures owned by the system.
 */
SpriteRenderSystem::~SpriteRenderSystem()
{
    for (auto& [assetPath, texture] : _textures) {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }
    }
}

/**
 * @brief Renders all drawable sprite entities in the ECS world.
 * @param ecs ECS world that owns the entities and components.
 * @param renderer SDL renderer that owns the current frame.
 */
void SpriteRenderSystem::render(const EntityComponentSystem& ecs, SDL_Renderer* renderer)
{
    if (renderer == nullptr) {
        return;
    }

    auto view = ecs.getRegistry().view<TransformComponent, SpriteComponent>();
    std::vector<entt::entity> sortedEntities;
    sortedEntities.reserve(view.size_hint());

    for (const auto entity : view) {
        sortedEntities.push_back(entity);
    }

    std::stable_sort(
        sortedEntities.begin(),
        sortedEntities.end(),
        [&view](const entt::entity left, const entt::entity right) {
            return view.get<SpriteComponent>(left).renderOrder < view.get<SpriteComponent>(right).renderOrder;
        }
    );

    for (const auto entity : sortedEntities) {
        const auto& transform = view.get<TransformComponent>(entity);
        const auto& sprite = view.get<SpriteComponent>(entity);
        SDL_Texture* texture = getTexture(renderer, sprite.assetPath);
        if (texture == nullptr) {
            continue;
        }

        SDL_FRect destinationRect {};
        destinationRect.w = sprite.size.x * transform.scale.x;
        destinationRect.h = sprite.size.y * transform.scale.y;
        destinationRect.x = transform.position.x - (destinationRect.w * 0.5F);
        destinationRect.y = transform.position.y - (destinationRect.h * 0.5F);

        SDL_FRect sourceRect {};
        sourceRect.x = sprite.sourcePosition.x;
        sourceRect.y = sprite.sourcePosition.y;
        sourceRect.w = sprite.sourceSize.x;
        sourceRect.h = sprite.sourceSize.y;

        SDL_RenderTextureRotated(
            renderer,
            texture,
            &sourceRect,
            &destinationRect,
            transform.rotation,
            nullptr,
            SDL_FLIP_NONE
        );
    }
}

/**
 * @brief Returns a cached texture for the given asset path.
 * @param renderer SDL renderer used to create textures.
 * @param assetPath Path to the texture asset on disk.
 * @return Texture pointer when loading succeeds, otherwise `nullptr`.
 */
SDL_Texture* SpriteRenderSystem::getTexture(SDL_Renderer* renderer, const std::string& assetPath)
{
    const auto iterator = _textures.find(assetPath);
    if (iterator != _textures.end()) {
        return iterator->second;
    }

    for (const auto& candidatePath : {std::filesystem::path(assetPath), std::filesystem::path("..") / assetPath}) {
        SDL_Texture* texture = IMG_LoadTexture(renderer, candidatePath.string().c_str());
        if (texture != nullptr) {
            _textures[assetPath] = texture;
            return texture;
        }
    }

    cbit2d::core::Logger::error("Couldn't load sprite texture '{}': {}", assetPath, SDL_GetError());
    return nullptr;
}

} // namespace cbit::ecs
