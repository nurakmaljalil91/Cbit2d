/**
 * @file CustomScene.cpp
 */

#include "CustomScene.h"
#include "../../src/utilities/Logger.h"

CustomScene::CustomScene() {}

CustomScene::~CustomScene() {}

void CustomScene::init() {
    LOG_INFO("CustomScene initialized");
}

void CustomScene::update() {
//    LOG_INFO("CustomScene updated");
}

void CustomScene::render() {}

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



