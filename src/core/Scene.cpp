/**
 * @file Scene.cpp
 * @brief The base class for all scenes in the game.
 *
 * This file contains the implementation of the Scene class which is the base class for all scenes in the game.
 * The Scene class is an abstract class that provides the basic structure for all scenes in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */

#include "Scene.h"

Scene::Scene() = default;

Scene::~Scene() = default;

void Scene::setup() {
    _registry.clear();
    stopBGM();
}

void Scene::update(Input &input) {
    // if debug mode is on, log the coordinates of the mouse when clicked
    int mouseX, mouseY;
    if (_isDebug && input.isMouseButtonPressed(SDL_MOUSEBUTTONDOWN)) {

        SDL_GetMouseState(&mouseX, &mouseY);
        LOG_INFO("Mouse clicked at ({}, {})", mouseX, mouseY);
    }

    auto buttonView = _registry.view<TransformComponent, ButtonComponent>();
    for (auto entity: buttonView) {
        auto &button = buttonView.get<ButtonComponent>(entity);
        auto &transform = buttonView.get<TransformComponent>(entity);
        // hover
        input.getMousePosition(mouseX, mouseY);
        if (mouseX != 0 || mouseY != 0) {
            if (mouseX >= transform.x && mouseX <= transform.x + transform.width &&
                mouseY >= transform.y && mouseY <= transform.y + transform.height) {
                button.isHover = true;
                LOG_INFO("Mouse hovered on button");
                SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
            } else {
                SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
                button.isHover = false;
            }
        }

        if (input.isMouseButtonPressed(SDL_MOUSEBUTTONDOWN)) {
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= transform.x && mouseX <= transform.x + transform.width &&
                mouseY >= transform.y && mouseY <= transform.y + transform.height) {
                button.onClick();
            }
        }
    }
}

void Scene::render(SDL_Renderer *renderer) {
    // Set the background color
    if (_isBackgroundColorSet) {
        SDL_SetRenderDrawColor(renderer, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b,
                               _backgroundColor.a);
        SDL_RenderClear(renderer);
    }

    // Render debug grid if _isDebug is true
    if (_isDebug) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for grid lines
        int gridSize = 64;

        // Get the window size
        int windowWidth, windowHeight;
        SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

        // Draw vertical lines
        for (int x = 0; x < windowWidth; x += gridSize) {
            SDL_RenderDrawLine(renderer, x, 0, x, windowHeight);
        }

        // Draw horizontal lines
        for (int y = 0; y < windowHeight; y += gridSize) {
            SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
        }
    }

    auto transformView = _registry.view<TransformComponent>();
    for (auto entity: transformView) {
        auto &transform = transformView.get<TransformComponent>(entity);
        if (_isDebug) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            // draw the bounding box
            SDL_Rect rect = {transform.x, transform.y, transform.width, transform.height};
            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    auto spriteView = _registry.view<TransformComponent, SpriteComponent>();
    for (auto entity: spriteView) {
        auto &position = spriteView.get<TransformComponent>(entity);
        auto &sprite = spriteView.get<SpriteComponent>(entity);
        SDL_Rect dstRect = {position.x, position.y, position.width, position.height};
        SDL_Texture *texture = AssetManager::getInstance().loadTexture(sprite.textureName);
        SDL_Rect srcRect = {sprite.x, sprite.y, sprite.width, sprite.height};
        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
    }

    auto textView = _registry.view<TransformComponent, TextComponent>();
    for (auto entity: textView) {
        auto &position = textView.get<TransformComponent>(entity);
        auto &text = textView.get<TextComponent>(entity);
        TTF_Font *font = AssetManager::getInstance().loadFont(text.fontName, text.size);
        SDL_Color color = {static_cast<Uint8>(text.textColor.r), static_cast<Uint8>(text.textColor.g),
                           static_cast<Uint8>(text.textColor.b), static_cast<Uint8>(text.textColor.a)};
        renderText(renderer, text.text.c_str(), font, position.x, position.y, position.width, position.height, color);
    }

    auto buttonView = _registry.view<TransformComponent, ButtonComponent>();
    for (auto entity: buttonView) {
        auto &position = buttonView.get<TransformComponent>(entity);
        auto &button = buttonView.get<ButtonComponent>(entity);
        if (button.isHover) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect rect = {position.x, position.y, position.width, position.height};
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

void Scene::cleanup() {
    _registry.clear();
}

void Scene::renderText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y, int width, int height,
                       SDL_Color color) {
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

bool Scene::switchScene() const {
    return _isChangeScene;
}

void Scene::changeScene(const std::string &name) {
    _isChangeScene = true;
    _nextScene = name;
}

std::string Scene::getNextScene() {
    _isChangeScene = false;
    return _nextScene;
}

void Scene::toggleDebug() {
    _isDebug = !_isDebug;
}

void Scene::setBackgroundColour(Color color) {
    _backgroundColor = color;
    _isBackgroundColorSet = true;
}

void Scene::playBGM(const std::string &name) {
    _bgm = AssetManager::getInstance().loadAudio(name);
    Mix_PlayMusic(_bgm, -1);
}

void Scene::stopBGM() {
    Mix_HaltMusic();
}

void Scene::playSFX(const std::string &name) {
    Mix_Chunk *sfx = AssetManager::getInstance().loadSound(name);
    Mix_PlayChannel(-1, sfx, 0);
}

