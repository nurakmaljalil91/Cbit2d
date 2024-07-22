/**
 * @file Scene.h
 * @brief The base class for all scenes in the game.
 *
 * This file contains the definition of the Scene class which is the base class for all scenes in the game.
 * The Scene class is an abstract class that provides the basic structure for all scenes in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */


#ifndef CBIT2D_SCENE_H
#define CBIT2D_SCENE_H

#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../utilities/Logger.h."
#include "Components.h"

class Scene {
public:
    Scene();

    virtual ~Scene();

    virtual void setup(SDL_Renderer *renderer);

    virtual void update();

    virtual void render(SDL_Renderer *renderer);

    virtual void handleInput(SDL_Event event);

    virtual void cleanup();

protected:
    entt::registry _registry;

    SDL_Texture *_loadTexture(SDL_Renderer *renderer, const char *path);
};


#endif //CBIT2D_SCENE_H
