//
// Created by User on 21/7/2024.
//

#include "SceneManager.h"

SceneManager::SceneManager() {
    init();
}

SceneManager::~SceneManager() {
    cleanup();
}

void SceneManager::init() {
    currentScene = nullptr;
}

void SceneManager::update() {
    if (currentScene) {
        currentScene->update();
    }
}

void SceneManager::render() {
    if (currentScene) {
        currentScene->render();
    }
}

void SceneManager::handleInput() {
    if (currentScene) {
        currentScene->handleInput();
    }
}

void SceneManager::cleanup() {
    if (currentScene) {
        currentScene->cleanup();
    }
}

void SceneManager::addScene(const std::string &name, std::shared_ptr<Scene> scene) {
    scenes[name] = scene;
    currentScene = scene;
}

void SceneManager::switchTo(const std::string &name) {
    if (scenes.find(name) != scenes.end()) {
        currentScene = scenes[name];
    }
}