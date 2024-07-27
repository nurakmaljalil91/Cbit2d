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
#include "Input.h"
#include "Components.h"
#include "AssetManager.h"
#include "../utilities/Logger.h"

class EntityComponentSystem {
public:
    EntityComponentSystem();

    ~EntityComponentSystem();

    void update(float deltaTime, Input &input, bool debug);

    void render(SDL_Renderer *renderer, bool debug);

    void cleanup();

    entt::registry registry;

    static void renderText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y, int width, int height,
                    SDL_Color color);
};




#endif //CBIT2D_ENTITYCOMPONENTSYSTEM_H
