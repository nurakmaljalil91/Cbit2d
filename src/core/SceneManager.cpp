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
    _currentScene = nullptr;
}

SceneManager::~SceneManager() {
    cleanup();
}

void SceneManager::setup(SDL_Renderer *renderer) {
    _renderer = renderer;
}

void SceneManager::update() {
    if (_currentScene) {
        _currentScene->update();
    }
}

void SceneManager::render(SDL_Renderer* renderer) {
    if (_currentScene) {
        _currentScene->render(renderer);
    }
}

void SceneManager::handleInput(SDL_Event event) {
    if (_currentScene) {
        _currentScene->handleInput(event);
    }
}

void SceneManager::cleanup() {
    if (_currentScene) {
        _currentScene->cleanup();
    }
}

void SceneManager::addScene(const std::string &name, std::shared_ptr<Scene> scene) {
    _scenes[name] = scene;
}

void SceneManager::switchTo(const std::string &name) {
    if (_scenes.find(name) != _scenes.end()) {
        _currentScene = _scenes[name];
    }
}

void SceneManager::setCurrentScene(const std::string &name) {
    if (_scenes.find(name) != _scenes.end()) {
        _currentScene = _scenes[name];
        _currentScene->setup(_renderer);
    }
}