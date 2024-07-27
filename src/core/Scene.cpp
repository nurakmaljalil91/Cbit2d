/**
 * @file Scene.cpp
 * @brief The base class for all scenes in the game.
 *
 * This file contains the implementation of the Scene class which is the base class for all scenes in the game.
 * The Scene class is an abstract class that provides the basic structure for all scenes in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */

#include "Scene.h"

Scene::Scene() = default;

Scene::~Scene() = default;

void Scene::setup() {
    _ecs.cleanup();
    stopBGM();
}

void Scene::update(float deltaTime, Input &input) {
    // if debug mode is on, log the coordinates of the mouse when clicked
    int mouseX, mouseY;
    if (_isDebug && input.isMouseButtonPressed(SDL_MOUSEBUTTONDOWN)) {

        SDL_GetMouseState(&mouseX, &mouseY);
        LOG_INFO("Mouse clicked at ({}, {})", mouseX, mouseY);
    }

    _ecs.update(deltaTime, input, _isDebug);
}

void Scene::render(SDL_Renderer *renderer) {
    // Set the background color
    if (_isBackgroundColorSet) {
        SDL_SetRenderDrawColor(renderer, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b,
                               _backgroundColor.a);
        SDL_RenderClear(renderer);
    }

    // Render debug grid if _isDebug is true
    if (_isDebug) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for grid lines
        int gridSize = 64;

        // Get the window size
        int windowWidth, windowHeight;
        SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

        // Draw vertical lines
        for (int x = 0; x < windowWidth; x += gridSize) {
            SDL_RenderDrawLine(renderer, x, 0, x, windowHeight);
        }

        // Draw horizontal lines
        for (int y = 0; y < windowHeight; y += gridSize) {
            SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
        }
    }

    _ecs.render(renderer, _isDebug);
}

void Scene::cleanup() {
    _ecs.cleanup();
}

bool Scene::switchScene() const {
    return _isChangeScene;
}

void Scene::changeScene(const std::string &name) {
    _isChangeScene = true;
    _nextScene = name;
}

std::string Scene::getNextScene() {
    _isChangeScene = false;
    return _nextScene;
}

void Scene::toggleDebug() {
    _isDebug = !_isDebug;
}

void Scene::setBackgroundColour(Color color) {
    _backgroundColor = color;
    _isBackgroundColorSet = true;
}

void Scene::playBGM(const std::string &name) {
    _bgm = AssetManager::getInstance().loadAudio(name);
    Mix_PlayMusic(_bgm, -1);
}

void Scene::stopBGM() {
    Mix_HaltMusic();
}

void Scene::playSFX(const std::string &name) {
    Mix_Chunk *sfx = AssetManager::getInstance().loadSound(name);
    Mix_PlayChannel(-1, sfx, 0);
}

