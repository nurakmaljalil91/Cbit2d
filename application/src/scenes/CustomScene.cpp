/**
 * @file CustomScene.cpp
 */

#include "CustomScene.h"

CustomScene::CustomScene() {}

CustomScene::~CustomScene() {}

void CustomScene::setup(SDL_Renderer *renderer) {
//    LOG_INFO("CustomScene initialized");
    _image = _registry.create();
    _registry.emplace<TransformComponent>(_image, 0, 0, 100, 100);
    SDL_Texture *texture = _loadTexture(renderer, "resources/test.png");
    _registry.emplace<SpriteComponent>(_image, texture, SDL_Rect{0, 0, 100, 100}, SDL_Rect{100, 100, 100, 100});
}

void CustomScene::update() {
//    LOG_INFO("CustomScene updated");
}


void CustomScene::handleInput(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                LOG_INFO("Escape key pressed");
                break;
            default:
                break;
        }
    }
}

void CustomScene::cleanup() {}



