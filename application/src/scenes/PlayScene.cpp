/**
 * @file PlayScene.cpp
 * @brief Implementation file for the PlayScene class.
 *
 * This file contains the implementation of the PlayScene class which is responsible for managing the play scene in the game.
 * The PlayScene class is responsible for setting up the play scene, updating the play scene, and handling input for the play scene.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */

#include "PlayScene.h"

PlayScene::PlayScene() : Scene() {}

PlayScene::~PlayScene() = default;

void PlayScene::setup() {
    Scene::setup();
    toggleDebug();
    auto _player = _registry.create();
    _registry.emplace<TransformComponent>(_player, 0, 0, 64, 64);
    _registry.emplace<SpriteComponent>(_player, "sokoban_tilesheet", 0, 256, 64, 64);
}

void PlayScene::update(Input &input) {
    Scene::update(input);
    if (input.isKeyPressed(SDLK_w)) {
        LOG_INFO("Moving up");
    }
    if (input.isKeyPressed(SDLK_RETURN)) {
        LOG_INFO("Changing scene to MenuScene");
        changeScene("MenuScene");

    }
}
