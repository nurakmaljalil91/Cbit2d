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

void Scene::setup() {}

void Scene::update() {}

void Scene::render(SDL_Renderer *renderer) {
    auto view = _registry.view<TransformComponent, SpriteComponent>();
    for (auto entity: view) {
        auto &position = view.get<TransformComponent>(entity);
        auto &sprite = view.get<SpriteComponent>(entity);
        SDL_Rect dstRect = {position.x, position.y, position.width, position.height};
        SDL_RenderCopy(renderer, sprite.texture, nullptr, &dstRect);
    }

    auto textView = _registry.view<TransformComponent, TextComponent>();
    for (auto entity: textView) {
        auto &position = textView.get<TransformComponent>(entity);
        auto &text = textView.get<TextComponent>(entity);
        renderText(renderer, text.text.c_str(), text.font, position.x, position.y, text.color);
    }
}

void Scene::handleInput(SDL_Event event) {}

void Scene::cleanup() {
    auto view = _registry.view<SpriteComponent>();
    for (auto entity: view) {
        auto &sprite = view.get<SpriteComponent>(entity);
        SDL_DestroyTexture(sprite.texture);
    }

    auto textView = _registry.view<TextComponent>();
    for (auto entity: textView) {
        auto &text = textView.get<TextComponent>(entity);
        TTF_CloseFont(text.font);
    }
    _registry.clear();
}

SDL_Texture *Scene::_loadTexture(SDL_Renderer *renderer, const char *path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (texture == nullptr) {
        LOG_ERROR("Unable to load texture {}! SDL_image Error: {}", path, IMG_GetError());
    }
    return texture;
}

SDL_Texture *
Scene::_createTextTexture(SDL_Renderer *renderer, const std::string &text, TTF_Font *font, SDL_Color color, int &width,
                          int &height) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr) {
        LOG_ERROR("Unable to render text surface! TTF_Error: {}", TTF_GetError());
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        LOG_ERROR("Unable to create texture from rendered text! SDL_Error: {}", SDL_GetError());
        SDL_FreeSurface(surface);
        return nullptr;
    }

    width = surface->w;
    height = surface->h;
    SDL_FreeSurface(surface);
    return texture;
}

void Scene::renderText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y, SDL_Color color) {
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

    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

bool Scene::switchScene() {
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

