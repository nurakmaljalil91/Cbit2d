/**
 * @file    sprite_render_system.hpp
 * @brief   Header file for the SpriteRenderSystem class.
 * @details This file contains the definition of the SpriteRenderSystem class
 *          which is responsible for drawing sprite components managed by the
 *          ECS world and caching SDL textures for reuse across frames.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>

namespace cbit::ecs {

class EntityComponentSystem;

/**
 * @brief Draws ECS sprite components through SDL.
 * @details This system scans sprite and transform components, loads textures
 *          on demand, caches them by asset path, and renders the selected
 *          source frame for each entity.
 */
class SpriteRenderSystem {
public:
    /**
     * @brief Releases any cached SDL textures owned by the system.
     */
    ~SpriteRenderSystem();

    /**
     * @brief Renders all drawable sprite entities in the ECS world.
     * @param ecs ECS world that owns the entities and components.
     * @param renderer SDL renderer that owns the current frame.
     */
    void render(const EntityComponentSystem& ecs, SDL_Renderer* renderer);

private:
    /**
     * @brief Returns a cached texture for the given asset path.
     * @param renderer SDL renderer used to create textures.
     * @param assetPath Path to the texture asset on disk.
     * @return Texture pointer when loading succeeds, otherwise `nullptr`.
     */
    SDL_Texture* getTexture(SDL_Renderer* renderer, const std::string& assetPath);

    std::unordered_map<std::string, SDL_Texture*> _textures;
};

} // namespace cbit::ecs
