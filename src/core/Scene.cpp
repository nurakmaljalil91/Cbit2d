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

Scene::Scene(){}

Scene::~Scene(){
    cleanup();
}

void Scene::init() {}

void Scene::update() {}

void Scene::render() {}

void Scene::handleInput(SDL_Event event) {}

void Scene::cleanup() {}
