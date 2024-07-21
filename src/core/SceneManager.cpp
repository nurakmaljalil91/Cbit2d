/**
 * @file SceneManager.cpp
 * @brief Implementation file for the SceneManager class.
 *
 * This file contains the implementation of the SceneManager class which manages the scenes in the game.
 * The SceneManager class is responsible for adding, switching, and updating the scenes in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */

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

void SceneManager::handleInput(SDL_Event event) {
    if (currentScene) {
        currentScene->handleInput(event);
    }
}

void SceneManager::cleanup() {
    if (currentScene) {
        currentScene->cleanup();
    }
}

void SceneManager::addScene(const std::string &name, std::shared_ptr<Scene> scene) {
    scenes[name] = scene;
}

void SceneManager::switchTo(const std::string &name) {
    if (scenes.find(name) != scenes.end()) {
        currentScene = scenes[name];
    }
}

void SceneManager::setCurrentScene(const std::string &name) {
    if (scenes.find(name) != scenes.end()) {
        currentScene = scenes[name];
        currentScene->init();
    }
}