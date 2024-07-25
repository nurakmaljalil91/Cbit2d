//
// Created by User on 22/7/2024.
//

#include "MenuScene.h"
#include <SDL2/SDL_mixer.h>
//#include "../../../src/core/SceneManager.h"
#include "../../../src/core/AssetManager.h"

MenuScene::MenuScene() : Scene() {

}

MenuScene::~MenuScene() = default;

void MenuScene::setup() {
    _titleText = _registry.create();
    TTF_Font *defaultFont = AssetManager::getInstance().loadFont("JetBrainsMono-Regular", 32);
    _registry.emplace<TextComponent>(_titleText, "CBIT2D", defaultFont, SDL_Color{255, 255, 255, 255});
    _registry.emplace<TransformComponent>(_titleText, 50, 50, 100, 50);
    _playButton = _registry.create();
    _registry.emplace<TransformComponent>(_playButton, 200, 200, 100, 100);
    SDL_Texture *texture = AssetManager::getInstance().loadTexture("sokoban_spritesheet");
    _registry.emplace<SpriteComponent>(_playButton, texture, SDL_Rect{0, 0, 100, 100}, SDL_Rect{100, 100, 100, 100});
    Mix_Music *bgm = AssetManager::getInstance().loadAudio("easy-cheesy-bgm");
    Mix_PlayMusic(bgm, -1);
}

void MenuScene::update() {
    Scene::update();
}

void MenuScene::render(SDL_Renderer *renderer) {
    Scene::render(renderer);
}

void MenuScene::handleInput(SDL_Event event) {
    Scene::handleInput(event);
    // change scene if click enter
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            LOG_INFO("Changing scene to PlayScene");
            changeScene("Play2Scene");
//            SceneManager::getInstance().setActiveScene("PlayScene");
        }
    }
}

void MenuScene::cleanup() {
    Scene::cleanup();
}
