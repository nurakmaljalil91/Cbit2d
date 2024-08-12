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

PlayScene::PlayScene() : Scene() {
}

PlayScene::~PlayScene() = default;

void PlayScene::setup() {
    Scene::setup();
//    toggleDebug();

    // load map
    _tileMap.loadMap("sokoban_tilesheet", "resources/maps/basic.json", 64, 64, _ecs);

    _player = _ecs.createGameObject("player");
    _player.addComponent<TransformComponent>(64, 64, 64, 64);
    _player.addComponent<ColliderComponent>("player", 45, 50);
    auto &animatedSprite = _player.addComponent<AnimatedSpriteComponent>("sokoban_tilesheet", 64, 64, 1);
    animatedSprite.addAnimation("idle", 0, 256, 1, 0.2f);
    animatedSprite.addAnimation("down", 0, 256, 3, 0.2f);
    animatedSprite.addAnimation("up", 192, 256, 3, 0.2f);
    animatedSprite.addAnimation("left", 192, 384, 3, 0.2f);
    animatedSprite.addAnimation("right", 0, 384, 3, 0.2f);
    animatedSprite.playAnimation("idle");

    _enemy = _ecs.createGameObject("enemy");
    _enemy.addComponent<TransformComponent>(192, 192, 64, 64);
    _enemy.addComponent<SpriteComponent>("sokoban_tilesheet", 64, 64, 64, 64, 2);
    _enemy.addComponent<ColliderComponent>("enemy", 64, 64);

    auto crate = _ecs.createGameObject("crate");
    crate.addComponent<TransformComponent>(128, 128, 64, 64);
    crate.addComponent<SpriteComponent>("sokoban_tilesheet", 64, 64, 64, 64, 3);
    crate.addComponent<DraggableComponent>();
}

void PlayScene::update(float deltaTime, Input &input) {
    Scene::update(deltaTime, input);

    // handle input for the player movement
    auto &transform = _player.getComponent<TransformComponent>();
    auto &animatedSprite = _player.getComponent<AnimatedSpriteComponent>();
    bool isMoving = false;
    if (input.isKeyHeld(SDLK_w)) {
        transform.velocity.x = 0;
        transform.velocity.y = -200;
        isMoving = true;
        animatedSprite.playAnimation("up");
    }
    if (input.isKeyHeld(SDLK_s)) {
        transform.velocity.x = 0;
        transform.velocity.y = 200;
        isMoving = true;
        animatedSprite.playAnimation("down");
    }
    if (input.isKeyHeld(SDLK_a)) {
        transform.velocity.x = -200;
        transform.velocity.y = 0;
        isMoving = true;
        animatedSprite.playAnimation("left");
    }
    if (input.isKeyHeld(SDLK_d)) {
        transform.velocity.x = 200;
        transform.velocity.y = 0;
        isMoving = true;
        animatedSprite.playAnimation("right");
    }

    // Update player position based on velocity
    transform.position.x += transform.velocity.x * deltaTime;
    transform.position.y += transform.velocity.y * deltaTime;

    auto &playerCollider = _player.getComponent<ColliderComponent>();

    // Center the collider relative to the transform's position and size
    playerCollider.x = static_cast<int>(transform.position.x ) + (transform.width - playerCollider.width) / 2;
    playerCollider.y = static_cast<int>(transform.position.y) + (transform.height - playerCollider.height) / 2;

    // Check for collision with walls
    auto view = _ecs.getAllEntitiesWith<TransformComponent, ColliderComponent>();

    for (auto entity: view) {
        if (entity == _player.getEntity()) continue;

        auto &otherTransform = view.get<TransformComponent>(entity);
        auto &otherCollider = view.get<ColliderComponent>(entity);


        if (otherCollider.tag == "wall" || otherCollider.tag == "enemy") {
            if (playerCollider.x < otherCollider.x + otherCollider.width &&
                playerCollider.x + playerCollider.width > otherCollider.x &&
                playerCollider.y < otherCollider.y + otherCollider.height &&
                playerCollider.y + playerCollider.height > otherCollider.y) {
//                LOG_INFO("Player collided with wall");
                // Resolve collision: Reset position to previous state
                transform.position.x -= transform.velocity.x * deltaTime;
                transform.position.y -= transform.velocity.y * deltaTime;
                transform.velocity.x = 0;
                transform.velocity.y = 0;
            }
        }
    }

    // Reset velocity if no movement key is pressed
    if (!isMoving) {
        transform.velocity.x = 0;
        transform.velocity.y = 0;
        animatedSprite.playAnimation("idle");
    }

    if (input.isKeyPressed(SDLK_RETURN)) {
        LOG_INFO("Changing scene to MenuScene");
        changeScene("MenuScene");

    }
}
