//
// Created by User on 22/7/2024.
//

#include "MenuScene.h"

MenuScene::MenuScene() : Scene() {

}

MenuScene::~MenuScene() {

}

void MenuScene::setup(SDL_Renderer *renderer) {
    _defaultFont = TTF_OpenFont("resources/fonts/Kenney_Future.ttf", 24);
    if (!_defaultFont) {
        LOG_ERROR("Failed to load font: %s", TTF_GetError());
    }

//    _titleText = _registry.create();
//    SDL_Texture* textTexture = _createTextTexture(renderer, "Menu Scene", _defaultFont, SDL_Color{255, 255, 255, 255}, 100, 50);
//    _registry.emplace<TextComponent>(_titleText, "CBIT2D", _defaultFont, SDL_Color{255, 255, 255, 255});
}

void MenuScene::update() {
    Scene::update();
}

void MenuScene::render(SDL_Renderer *renderer) {
//    Scene::render(renderer);
    renderText(renderer, "Hello, World!", _defaultFont, 50, 50, {255, 255, 255, 255});
}

void MenuScene::handleInput(SDL_Event event) {
    Scene::handleInput(event);
}

void MenuScene::cleanup() {
    Scene::cleanup();
}
