/**
 * @file EntityComponentSystem.h
 * @brief Header file for the EntityComponentSystem class.
 *
 * This file contains the definition of the EntityComponentSystem class which is responsible for managing the entity component system in the game.
 * The EntityComponentSystem class is responsible for adding, removing, and updating the entities and components in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-27
 */

#ifndef CBIT2D_ENTITYCOMPONENTSYSTEM_H
#define CBIT2D_ENTITYCOMPONENTSYSTEM_H

#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <random>
#include <iomanip>
#include <sstream>
#include "Input.h"
#include "Components.h"
#include "AssetManager.h"
#include "../utilities/Logger.h"

class GameObject;

class EntityComponentSystem {
public:
    EntityComponentSystem();

    ~EntityComponentSystem();

    void update(float deltaTime, Input &input);

    void render(SDL_Renderer *renderer, bool debug);

    void cleanup();

    GameObject createGameObject(const std::string &tag);

    void destroyGameObject(GameObject entity);

    GameObject getGameObjectByName(const std::string &name);

    template<typename... Components>
    auto getAllEntitiesWith() {
        return _registry.view<Components...>();
    }

private:
    entt::registry _registry;

    friend class GameObject;

    std::string generateUUID();

    static void
    renderText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y, int width, int height,
               SDL_Color color);

};


#endif //CBIT2D_ENTITYCOMPONENTSYSTEM_H
