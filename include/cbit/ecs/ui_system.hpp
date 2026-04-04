/**
 * @file    ui_system.hpp
 * @brief   Header file for the UISystem class.
 * @details This file contains the definition of the UISystem class which is
 *          responsible for updating and drawing ECS-driven UI text and button
 *          elements.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace cbit::ecs {

class EntityComponentSystem;

/**
 * @brief Updates and draws ECS-managed UI button and text components.
 * @details This system processes button clicks, caches opened fonts, renders
 *          button rectangles, and draws text labels through SDL_ttf.
 */
class UISystem {
public:
    /**
     * @brief Releases cached font resources owned by the system.
     */
    ~UISystem();

    /**
     * @brief Updates UI button interactions for the ECS world.
     * @param ecs ECS world that owns the entities and components.
     */
    void update(const EntityComponentSystem& ecs);

    /**
     * @brief Renders all UI button and text components in the ECS world.
     * @param ecs ECS world that owns the entities and components.
     * @param renderer SDL renderer that owns the current frame.
     */
    void render(const EntityComponentSystem& ecs, SDL_Renderer* renderer);

private:
    /**
     * @brief Ensures SDL_ttf has been initialized for this process.
     * @return `true` when SDL_ttf is ready to use, otherwise `false`.
     */
    bool ensureTtfInitialized();

    /**
     * @brief Returns a cached font for the given path and size.
     * @param fontPath Path to the font file.
     * @param fontSize Point size used to open the font.
     * @return Font pointer when loading succeeds, otherwise `nullptr`.
     */
    TTF_Font* getFont(const std::string& fontPath, float fontSize);

    /**
     * @brief Renders a single text component.
     * @param renderer SDL renderer that owns the current frame.
     * @param text Text content and styling to render.
     * @param transform Transform used to position the rendered text.
     */
    void renderText(
        SDL_Renderer* renderer,
        const struct TextComponent& text,
        const struct TransformComponent& transform
    );

    /**
     * @brief Returns whether the current cursor is inside a button rectangle.
     * @param button Button visual definition to test.
     * @param transform Transform used to position the button.
     * @return `true` when the cursor is inside the button bounds.
     */
    bool isHovered(
        const struct ButtonComponent& button,
        const struct TransformComponent& transform
    ) const;

    /**
     * @brief Returns a font cache key for the provided font data.
     * @param fontPath Path to the font file.
     * @param fontSize Point size used to open the font.
     * @return Stable key for the font cache.
     */
    std::string buildFontKey(const std::string& fontPath, float fontSize) const;

    std::unordered_map<std::string, TTF_Font*> _fonts;
    bool _ownsTtfInitialization = false;
};

} // namespace cbit::ecs
