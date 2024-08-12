/**
 * @file EntityComponentSystem.cpp
 * @brief Implementation file for the EntityComponentSystem class.
 *
 * This file contains the implementation of the EntityComponentSystem class which is responsible for managing the entity component system in the game.
 * The EntityComponentSystem class is responsible for adding, removing, and updating the entities and components in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-27
 */

#include "EntityComponentSystem.h"
#include "GameObject.h"

EntityComponentSystem::EntityComponentSystem() = default;

EntityComponentSystem::~EntityComponentSystem() = default;

void EntityComponentSystem::update(float deltaTime, Input &input) {
    int mouseX, mouseY;
    auto colliderView = _registry.view<TransformComponent, ColliderComponent>();
    for (auto entity: colliderView) {
        auto &transform = colliderView.get<TransformComponent>(entity);
        auto &collider = colliderView.get<ColliderComponent>(entity);
        collider.x = static_cast<int>(transform.position.x) + (transform.width - collider.width) / 2;
        collider.y = static_cast<int>(transform.position.y) + (transform.height - collider.height) / 2;
    }

    auto buttonView = _registry.view<TransformComponent, ButtonComponent>();
    for (auto entity: buttonView) {
        auto &button = buttonView.get<ButtonComponent>(entity);
        auto &transform = buttonView.get<TransformComponent>(entity);
        // hover
        input.getMousePosition(mouseX, mouseY);
        if (mouseX != 0 || mouseY != 0) {
            if (mouseX >= static_cast<int>(transform.position.x) &&
                mouseX <= static_cast<int>(transform.position.x) + transform.width &&
                mouseY >= static_cast<int>(transform.position.y) &&
                mouseY <= static_cast<int>(transform.position.y) + transform.height) {
                button.isHover = true;
                SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
            } else {
                SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
                button.isHover = false;
            }
        }

        if (input.isMouseButtonPressed(SDL_BUTTON_LEFT)) {
            if (button.isHover) {
                button.onClick();
                SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
            }
        }
    }

    auto animatedSpriteView = _registry.view<AnimatedSpriteComponent>();
    for (auto entity: animatedSpriteView) {
        auto &animatedSprite = animatedSpriteView.get<AnimatedSpriteComponent>(entity);
        animatedSprite.currentTime += deltaTime;
        if (animatedSprite.currentTime >= animatedSprite.getCurrentAnimation().frameTime) {
            animatedSprite.currentFrame =
                    (animatedSprite.currentFrame + 1) % animatedSprite.getCurrentAnimation().frameCount;
            animatedSprite.currentTime = 0;
        }
    }

    auto draggableView = _registry.view<TransformComponent, DraggableComponent>();
    for (auto entity: draggableView) {
        auto &transform = draggableView.get<TransformComponent>(entity);
        auto &draggable = draggableView.get<DraggableComponent>(entity);
        input.getMousePosition(mouseX, mouseY);

        if (input.isMouseButtonPressed(SDL_BUTTON_LEFT)) {
            if (!draggable.isDragging) {
                // Check if the mouse is within the bounds of the entity
                if (mouseX >= static_cast<int>(transform.position.x) &&
                    mouseX <= static_cast<int>(transform.position.x) + transform.width &&
                    mouseY >= static_cast<int>(transform.position.y) &&
                    mouseY <= static_cast<int>(transform.position.y) + transform.height) {

                    draggable.isDragging = true;
                    // Calculate the offset when the dragging starts
                    draggable.offset.x = static_cast<float>(mouseX) - transform.position.x;
                    draggable.offset.y = static_cast<float>(mouseY) - transform.position.y;
                }
            }
        }

        if (input.isMouseButtonReleased(SDL_BUTTON_LEFT)) {
            draggable.isDragging = false;
        }

        if (draggable.isDragging) {
            // Update the entity's position based on the mouse position and the offset
            transform.position.x = static_cast<float>(mouseX) - draggable.offset.x;
            transform.position.y = static_cast<float>(mouseY) - draggable.offset.y;
        }
    }

    auto clickableView = _registry.view<TransformComponent, ClickableComponent>();
    for (auto entity: clickableView) {
        auto &transform = clickableView.get<TransformComponent>(entity);
        auto &clickable = clickableView.get<ClickableComponent>(entity);
        input.getMousePosition(mouseX, mouseY);
        if (mouseX >= static_cast<int>(transform.position.x) &&
            mouseX <= static_cast<int>(transform.position.x) + transform.width &&
            mouseY >= static_cast<int>(transform.position.y) &&
            mouseY <= static_cast<int>(transform.position.y) + transform.height) {
            clickable.isHover = true;
            if (input.isMouseButtonPressed(SDL_BUTTON_LEFT)) {
                clickable.isClicked = true;
            }
        } else {
            clickable.isHover = false;
        }
    }
}

void EntityComponentSystem::render(SDL_Renderer *renderer, bool debug) {
    auto spriteView = _registry.view<TransformComponent, SpriteComponent>();

    // Create a vector of entities to sort based on their layer
    std::vector<entt::entity> spriteEntities(spriteView.begin(), spriteView.end());

    // Sort entities by the layer value
    std::sort(spriteEntities.begin(), spriteEntities.end(), [&](entt::entity a, entt::entity b) {
        auto &transformA = spriteView.get<SpriteComponent>(a);
        auto &transformB = spriteView.get<SpriteComponent>(b);
        return transformA.layer < transformB.layer;
    });

    for (auto entity: spriteEntities) {
        auto &transform = spriteView.get<TransformComponent>(entity);
        auto &sprite = spriteView.get<SpriteComponent>(entity);
        SDL_Rect dstRect = {static_cast<int>(transform.position.x), static_cast<int>(transform.position.y),
                            transform.width, transform.height};
        SDL_Texture *texture = AssetManager::getInstance().loadTexture(sprite.textureName);
        SDL_Rect srcRect = {sprite.x, sprite.y, sprite.width, sprite.height};
        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
    }

    auto multipleView = _registry.view<TransformComponent, MultipleSpriteComponent>();

    // Create a vector of entities to sort based on their layer
    std::vector<entt::entity> multipleSpriteEntities(multipleView.begin(), multipleView.end());

    // Sort entities by the layer value
    std::sort(multipleSpriteEntities.begin(), multipleSpriteEntities.end(), [&](entt::entity a, entt::entity b) {
        auto &transformA = multipleView.get<MultipleSpriteComponent>(a);
        auto &transformB = multipleView.get<MultipleSpriteComponent>(b);
        return transformA.layer < transformB.layer;
    });

    for (auto entity: multipleSpriteEntities) {
        auto &transform = multipleView.get<TransformComponent>(entity);
        auto &multipleSprite = multipleView.get<MultipleSpriteComponent>(entity);
        SDL_Rect dstRect = {static_cast<int>(transform.position.x), static_cast<int>(transform.position.y),
                            transform.width, transform.height};
        SDL_Texture *texture = AssetManager::getInstance().loadTexture(
                multipleSprite.sprites[multipleSprite.currentSprite].textureName);
        SDL_Rect srcRect = {multipleSprite.sprites[multipleSprite.currentSprite].x,
                            multipleSprite.sprites[multipleSprite.currentSprite].y,
                            multipleSprite.sprites[multipleSprite.currentSprite].width,
                            multipleSprite.sprites[multipleSprite.currentSprite].height};
        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
    }

    auto animatedSpriteView = _registry.view<TransformComponent, AnimatedSpriteComponent>();

    // Create a vector of entities to sort based on their layer
    std::vector<entt::entity> animatedSpriteEntities(animatedSpriteView.begin(), animatedSpriteView.end());

    // Sort entities by the layer value
    std::sort(animatedSpriteEntities.begin(), animatedSpriteEntities.end(), [&](entt::entity a, entt::entity b) {
        auto &transformA = animatedSpriteView.get<AnimatedSpriteComponent>(a);
        auto &transformB = animatedSpriteView.get<AnimatedSpriteComponent>(b);
        return transformA.layer < transformB.layer;
    });

    for (auto entity: animatedSpriteEntities) {
        auto &transform = animatedSpriteView.get<TransformComponent>(entity);
        auto &animatedSprite = animatedSpriteView.get<AnimatedSpriteComponent>(entity);
        SDL_Rect dstRect = {static_cast<int>(transform.position.x), static_cast<int>(transform.position.y),
                            transform.width, transform.height};
        SDL_Texture *texture = AssetManager::getInstance().loadTexture(animatedSprite.textureName);
        SDL_Rect srcRect = {animatedSprite.getCurrentAnimation().x + animatedSprite.currentFrame * animatedSprite.width,
                            animatedSprite.getCurrentAnimation().y,
                            animatedSprite.width, animatedSprite.height};
        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);


    }

    auto textView = _registry.view<TransformComponent, TextComponent>();
    for (auto entity: textView) {
        auto &transform = textView.get<TransformComponent>(entity);
        auto &text = textView.get<TextComponent>(entity);
        TTF_Font *font = AssetManager::getInstance().loadFont(text.fontName, text.size);
        SDL_Color color = {static_cast<Uint8>(text.textColor.r), static_cast<Uint8>(text.textColor.g),
                           static_cast<Uint8>(text.textColor.b), static_cast<Uint8>(text.textColor.a)};
        renderText(renderer, text.text.c_str(), font, static_cast<int>(transform.position.x),
                   static_cast<int>(transform.position.y), transform.width,
                   transform.height, color);
    }

    auto buttonView = _registry.view<TransformComponent, ButtonComponent>();
    for (auto entity: buttonView) {
        auto &transform = buttonView.get<TransformComponent>(entity);
        auto &button = buttonView.get<ButtonComponent>(entity);
        if (button.isHover) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect rect = {static_cast<int>(transform.position.x), static_cast<int>(transform.position.y),
                             transform.width, transform.height};
            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    if (debug) {
        auto colliderView = _registry.view<TransformComponent, ColliderComponent>();
        for (auto entity: colliderView) {
            auto &transform = colliderView.get<TransformComponent>(entity);
            auto &collider = colliderView.get<ColliderComponent>(entity);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            // draw the bounding box
            SDL_Rect rect = {static_cast<int>(transform.position.x), static_cast<int>(transform.position.y),
                             transform.width, transform.height};
            SDL_RenderDrawRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            // draw the collider box
            SDL_Rect colliderRect = {collider.x, collider.y, collider.width, collider.height};
            SDL_RenderDrawRect(renderer, &colliderRect);
        }
    }
}

void
EntityComponentSystem::renderText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y, int width,
                                  int height, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (surface == nullptr) {
        LOG_ERROR("Unable to render text surface! TTF_Error: {}", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        LOG_ERROR("Unable to create texture from rendered text! SDL_Error: {}", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }


    // Calculate the center position
    int textWidth = surface->w;
    int textHeight = surface->h;
    int centerX = x + (width - textWidth) / 2;
    int centerY = y + (height - textHeight) / 2;

    SDL_Rect dstRect = {centerX, centerY, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void EntityComponentSystem::cleanup() {
    _registry.clear();
}

GameObject EntityComponentSystem::createGameObject(const std::string &tag) {
    GameObject entity = GameObject(_registry.create(), this);
    entity.addComponent<TagComponent>(tag);
    entity.addComponent<IdComponent>(generateUUID());
    return entity;
}

void EntityComponentSystem::destroyGameObject(GameObject entity) {
    _registry.destroy(entity.getEntity());
}

GameObject EntityComponentSystem::getGameObjectByName(const std::string &name) {
    auto view = _registry.view<TagComponent>();
    for (auto entity: view) {
        auto &tag = view.get<TagComponent>(entity);
        if (tag.tag == name) {
            return {entity, this};
        }
    }
    return {entt::null, this};
}

std::string EntityComponentSystem::generateUUID() {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 15);
    std::uniform_int_distribution<int> dist2(8, 11);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (int i = 0; i < 8; i++) {
        ss << std::setw(1) << dist(rd);
    }
    ss << "-";
    for (int i = 0; i < 4; i++) {
        ss << std::setw(1) << dist(rd);
    }
    ss << "-4"; // Version 4 UUID
    for (int i = 0; i < 3; i++) {
        ss << std::setw(1) << dist(rd);
    }
    ss << "-";
    ss << std::setw(1) << dist2(rd); // Ensure the leading bit is 10xx
    for (int i = 0; i < 3; i++) {
        ss << std::setw(1) << dist(rd);
    }
    ss << "-";
    for (int i = 0; i < 12; i++) {
        ss << std::setw(1) << dist(rd);
    }

    return ss.str();
}



