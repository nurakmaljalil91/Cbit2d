//
// Created by User on 22/7/2024.
//

#include "PlayScene.h"

PlayScene::PlayScene() = default;

PlayScene::~PlayScene() = default;


void PlayScene::setup() {
    Scene::setup();
}

void PlayScene::update() {
    Scene::update();
}

void PlayScene::render(SDL_Renderer *renderer) {
    Scene::render(renderer);
}

void PlayScene::handleInput(SDL_Event event) {
    Scene::handleInput(event);
    // change scene if click enter
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            LOG_INFO("Changing scene to PlayScene");
            changeScene("MenuScene");
//            SceneManager::getInstance().setActiveScene("PlayScene");
        }
    }
}

void PlayScene::cleanup() {
    Scene::cleanup();
}

