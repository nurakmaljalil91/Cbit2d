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

MenuScene::MenuScene() : Scene() {
}

MenuScene::~MenuScene() = default;

void MenuScene::setup() {
    Scene::setup();
//    toggleDebug();
    // set background color to yellow
    setBackgroundColour(Color{55, 138, 249, 255});
    // set background music
    playBGM("easy-cheesy-bgm");

    _titleText = _registry.create();
//    _sprite = _registry.create();
    _playButton = _registry.create();

    _registry.emplace<TextComponent>(_titleText, "Sokoban", "JetBrainsMono-Regular", 32, 255, 255, 255, 255);
    _registry.emplace<TransformComponent>(_titleText, 314, 221, 100, 50);

//    _registry.emplace<TransformComponent>(_sprite, 200, 200, 64, 64);
//    _registry.emplace<SpriteComponent>(_sprite, "sokoban_spritesheet", 64, 64, 64, 64);

    _registry.emplace<TransformComponent>(_playButton, 291, 283, 150, 50);
    _registry.emplace<TextComponent>(_playButton, "Play", "Kenney_Future", 32, Color{255, 255, 255, 255});
    _registry.emplace<SpriteComponent>(_playButton, "button_rectangle_depth_flat", 0, 0, 384, 128);
    _registry.emplace<ButtonComponent>(_playButton, [this]() {
        LOG_INFO("Changing scene to PlayScene");
        playSFX("click-a");
        changeScene("PlayScene");
    });
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


