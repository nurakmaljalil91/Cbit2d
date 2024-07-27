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
    _player = _registry.create();
    _registry.emplace<TransformComponent>(_player, 0, 0, 64, 64);
    _registry.emplace<SpriteComponent>(_player, "sokoban_tilesheet", 0, 256, 64, 64);
    _registry.emplace<ColliderComponent>(_player, 0, 0, 60, 60, 4, 4);

    _enemy = _registry.create();
    _registry.emplace<TransformComponent>(_enemy, 100, 100, 64, 64);
    _registry.emplace<SpriteComponent>(_enemy, "sokoban_tilesheet", 0, 256, 64, 64);
    _registry.emplace<ColliderComponent>(_enemy,0, 0, 58, 58, 4, 4);
}

void PlayScene::update(float deltaTime, Input &input) {
    Scene::update(deltaTime, input);

    // handle input for the player movement
    auto &transform = _registry.get<TransformComponent>(_player);
    bool isMoving = false;
    if (input.isKeyPressed(SDLK_w)) {
        transform.velocity.x = 0;
        transform.velocity.y = -200;
        isMoving = true;
    }
    if (input.isKeyPressed(SDLK_s)) {
        transform.velocity.x = 0;
        transform.velocity.y = 200;
        isMoving = true;
    }
    if (input.isKeyPressed(SDLK_a)) {
        transform.velocity.x = -200;
        transform.velocity.y = 0;
        isMoving = true;
    }
    if (input.isKeyPressed(SDLK_d)) {
        transform.velocity.x = 200;
        transform.velocity.y = 0;
        isMoving = false;
    }

    // Update player position based on velocity
    transform.position.x += transform.velocity.x * deltaTime;
    transform.position.y += transform.velocity.y * deltaTime;


    auto &playerCollider = _registry.get<ColliderComponent>(_player);
    auto &enemyCollider = _registry.get<ColliderComponent>(_enemy);

    playerCollider.x = static_cast<int>(transform.position.x) + playerCollider.offsetX;
    playerCollider.y = static_cast<int>(transform.position.y) + playerCollider.offsetY;

    if (playerCollider.x < enemyCollider.x + enemyCollider.width &&
        playerCollider.x + playerCollider.width > enemyCollider.x &&
        playerCollider.y < enemyCollider.y + enemyCollider.height &&
        playerCollider.y + playerCollider.height > enemyCollider.y) {
        LOG_INFO("Player collided with enemy");
        // Resolve collision: Reset position to previous state
        transform.position.x -= transform.velocity.x * deltaTime;
        transform.position.y -= transform.velocity.y * deltaTime;
        transform.velocity.x = 0;
        transform.velocity.y = 0;
    }

    // Reset velocity if no movement key is pressed
    if (!isMoving) {
        transform.velocity.x = 0;
        transform.velocity.y = 0;
    }

    if (input.isKeyPressed(SDLK_RETURN)) {
        LOG_INFO("Changing scene to MenuScene");
        changeScene("MenuScene");

    }
}
