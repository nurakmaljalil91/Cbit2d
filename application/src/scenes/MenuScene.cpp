/**
 * @file MenuScene.cpp
 * @brief Implementation file for the MenuScene class.
 *
 * This file contains the implementation of the MenuScene class which is responsible for managing the menu scene in the game.
 * The MenuScene class is responsible for setting up the menu scene, updating the menu scene, and handling input for the menu scene.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */

#include "MenuScene.h"
#include <SDL2/SDL_mixer.h>

MenuScene::MenuScene() : Scene() {
    toggleDebug();
    _titleText = _registry.create();
    _sprite = _registry.create();
    _playButton = _registry.create();
}

MenuScene::~MenuScene() = default;

void MenuScene::setup() {
    _registry.emplace<TextComponent>(_titleText, "CBIT2D", "JetBrainsMono-Regular", 32, 255, 255, 255, 255);
    _registry.emplace<TransformComponent>(_titleText, 50, 50, 100, 50);

    _registry.emplace<TransformComponent>(_sprite, 200, 200, 64, 64);
    _registry.emplace<SpriteComponent>(_sprite, "sokoban_spritesheet", 64, 64, 64, 64);

    _registry.emplace<TransformComponent>(_playButton, 400, 400, 150, 50);
    _registry.emplace<TextComponent>(_playButton, "Play", "Kenney_Future", 32, 255, 255, 255, 255);
    _registry.emplace<ButtonComponent>(_playButton, [this]() {
        LOG_INFO("Button clicked");
        LOG_INFO("Changing scene to PlayScene");
        changeScene("PlayScene");
    });
//    Mix_Music *bgm = AssetManager::getInstance().loadAudio("easy-cheesy-bgm");
//    Mix_PlayMusic(bgm, -1);
}

void MenuScene::update() {
    Scene::update();
}

void MenuScene::handleInput(SDL_Event event) {
    Scene::handleInput(event);
    // change scene if click enter
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            LOG_INFO("Changing scene to PlayScene");
            changeScene("PlayScene");
        }
    }
}


