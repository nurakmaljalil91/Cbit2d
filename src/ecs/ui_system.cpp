/**
 * @file    ui_system.cpp
 * @brief   Source file for the UISystem class.
 * @details This file contains the implementation of the UISystem class which
 *          is responsible for updating and rendering ECS-driven UI buttons and
 *          text.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#include "cbit/ecs/ui_system.hpp"

#include <filesystem>

#include "cbit/core/input.hpp"
#include "cbit/core/logger.hpp"
#include "cbit/ecs/components.hpp"
#include "cbit/ecs/entity_component_system.hpp"

namespace cbit::ecs {

/**
 * @brief Releases cached font resources owned by the system.
 */
UISystem::~UISystem()
{
    for (auto& [fontKey, font] : _fonts) {
        if (font != nullptr) {
            TTF_CloseFont(font);
        }
    }

    if (_ownsTtfInitialization) {
        TTF_Quit();
    }
}

/**
 * @brief Updates UI button interactions for the ECS world.
 * @param ecs ECS world that owns the entities and components.
 */
void UISystem::update(const EntityComponentSystem& ecs)
{
    if (!cbit2d::core::Input::isMouseButtonPressed(cbit2d::core::MouseButton::Left)) {
        return;
    }

    const auto buttonView = ecs.getRegistry().view<const TransformComponent, const ButtonComponent>();
    for (const auto entity : buttonView) {
        const auto& transform = buttonView.get<const TransformComponent>(entity);
        const auto& button = buttonView.get<const ButtonComponent>(entity);
        if (!isHovered(button, transform) || !button.onClick) {
            continue;
        }

        button.onClick();
        return;
    }
}

/**
 * @brief Renders all UI button and text components in the ECS world.
 * @param ecs ECS world that owns the entities and components.
 * @param renderer SDL renderer that owns the current frame.
 */
void UISystem::render(const EntityComponentSystem& ecs, SDL_Renderer* renderer)
{
    if (renderer == nullptr || !ensureTtfInitialized()) {
        return;
    }

    const auto buttonView = ecs.getRegistry().view<const TransformComponent, const ButtonComponent>();
    for (const auto entity : buttonView) {
        const auto& transform = buttonView.get<const TransformComponent>(entity);
        const auto& button = buttonView.get<const ButtonComponent>(entity);

        SDL_FRect destinationRect {};
        destinationRect.w = button.size.x * transform.scale.x;
        destinationRect.h = button.size.y * transform.scale.y;
        destinationRect.x = transform.position.x - (destinationRect.w * 0.5F);
        destinationRect.y = transform.position.y - (destinationRect.h * 0.5F);

        const bool hovered = isHovered(button, transform);
        SDL_Color fillColor = button.backgroundColor;
        if (hovered && cbit2d::core::Input::isMouseButtonHeld(cbit2d::core::MouseButton::Left)) {
            fillColor = button.pressedColor;
        } else if (hovered) {
            fillColor = button.hoverColor;
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
        SDL_RenderFillRect(renderer, &destinationRect);
        SDL_SetRenderDrawColor(
            renderer,
            button.borderColor.r,
            button.borderColor.g,
            button.borderColor.b,
            button.borderColor.a
        );
        SDL_RenderRect(renderer, &destinationRect);
    }

    const auto textView = ecs.getRegistry().view<const TransformComponent, const TextComponent>();
    for (const auto entity : textView) {
        renderText(
            renderer,
            textView.get<const TextComponent>(entity),
            textView.get<const TransformComponent>(entity)
        );
    }
}

/**
 * @brief Ensures SDL_ttf has been initialized for this process.
 * @return `true` when SDL_ttf is ready to use, otherwise `false`.
 */
bool UISystem::ensureTtfInitialized()
{
    if (TTF_WasInit() > 0) {
        return true;
    }

    if (!TTF_Init()) {
        cbit2d::core::Logger::error("Couldn't initialize SDL_ttf: {}", SDL_GetError());
        return false;
    }

    _ownsTtfInitialization = true;
    return true;
}

/**
 * @brief Returns a cached font for the given path and size.
 * @param fontPath Path to the font file.
 * @param fontSize Point size used to open the font.
 * @return Font pointer when loading succeeds, otherwise `nullptr`.
 */
TTF_Font* UISystem::getFont(const std::string& fontPath, float fontSize)
{
    const std::string fontKey = buildFontKey(fontPath, fontSize);
    const auto iterator = _fonts.find(fontKey);
    if (iterator != _fonts.end()) {
        return iterator->second;
    }

    for (const auto& candidatePath : {std::filesystem::path(fontPath), std::filesystem::path("..") / fontPath}) {
        TTF_Font* font = TTF_OpenFont(candidatePath.string().c_str(), fontSize);
        if (font != nullptr) {
            _fonts[fontKey] = font;
            return font;
        }
    }

    cbit2d::core::Logger::error("Couldn't load font '{}': {}", fontPath, SDL_GetError());
    return nullptr;
}

/**
 * @brief Renders a single text component.
 * @param renderer SDL renderer that owns the current frame.
 * @param text Text content and styling to render.
 * @param transform Transform used to position the rendered text.
 */
void UISystem::renderText(
    SDL_Renderer* renderer,
    const TextComponent& text,
    const TransformComponent& transform
)
{
    TTF_Font* font = getFont(text.fontPath, text.fontSize);
    if (font == nullptr) {
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(
        font,
        text.content.c_str(),
        0,
        text.color,
        text.wrapWidth
    );
    if (surface == nullptr) {
        cbit2d::core::Logger::error("Couldn't render UI text '{}': {}", text.content, SDL_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        cbit2d::core::Logger::error("Couldn't create UI text texture '{}': {}", text.content, SDL_GetError());
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect destinationRect {};
    destinationRect.w = static_cast<float>(surface->w) * transform.scale.x;
    destinationRect.h = static_cast<float>(surface->h) * transform.scale.y;
    destinationRect.x = transform.position.x;
    destinationRect.y = transform.position.y;

    if (text.centered) {
        destinationRect.x -= destinationRect.w * 0.5F;
        destinationRect.y -= destinationRect.h * 0.5F;
    }

    SDL_RenderTexture(renderer, texture, nullptr, &destinationRect);
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

/**
 * @brief Returns whether the current cursor is inside a button rectangle.
 * @param button Button visual definition to test.
 * @param transform Transform used to position the button.
 * @return `true` when the cursor is inside the button bounds.
 */
bool UISystem::isHovered(
    const ButtonComponent& button,
    const TransformComponent& transform
) const
{
    const SDL_FPoint mousePosition = cbit2d::core::Input::getMousePosition();
    const float halfWidth = (button.size.x * transform.scale.x) * 0.5F;
    const float halfHeight = (button.size.y * transform.scale.y) * 0.5F;

    return mousePosition.x >= transform.position.x - halfWidth
        && mousePosition.x <= transform.position.x + halfWidth
        && mousePosition.y >= transform.position.y - halfHeight
        && mousePosition.y <= transform.position.y + halfHeight;
}

/**
 * @brief Returns a font cache key for the provided font data.
 * @param fontPath Path to the font file.
 * @param fontSize Point size used to open the font.
 * @return Stable key for the font cache.
 */
std::string UISystem::buildFontKey(const std::string& fontPath, float fontSize) const
{
    return fontPath + "#" + std::to_string(fontSize);
}

} // namespace cbit::ecs
