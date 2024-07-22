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

Scene::Scene() {}

Scene::~Scene() {
    cleanup();
}

void Scene::setup(SDL_Renderer *renderer) {}

void Scene::update() {}

void Scene::render(SDL_Renderer *renderer) {
    LOG_INFO("Rendering scene");
    auto view = _registry.view<TransformComponent, SpriteComponent>();
    for (auto entity: view) {
        auto &position = view.get<TransformComponent>(entity);
        auto &sprite = _registry.get<SpriteComponent>(entity);
        SDL_Rect dstRect = {position.x, position.y, position.width, position.height};
        SDL_RenderCopy(renderer, sprite.texture, nullptr, &dstRect);
    }
}

void Scene::handleInput(SDL_Event event) {}

void Scene::cleanup() {
    auto view = _registry.view<SpriteComponent>();
    for (auto entity: view) {
        auto &sprite = view.get<SpriteComponent>(entity);
        SDL_DestroyTexture(sprite.texture);
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
