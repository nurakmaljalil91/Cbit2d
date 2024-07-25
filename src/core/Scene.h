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
#include <SDL2/SDL_ttf.h>
#include "../utilities/Logger.h."
#include "Components.h"
#include "AssetManager.h"

class Scene {
public:
    Scene();

    virtual ~Scene();

    // scene lifecycle
    virtual void setup();

    virtual void update();

    void render(SDL_Renderer *renderer);

    virtual void handleInput(SDL_Event event);

    void cleanup();

    // handle scene change
    bool switchScene() const;

    void changeScene(const std::string &name);

    std::string getNextScene();

    // debugging
    /**
     * @brief Toggle the debug mode.
     */
    void toggleDebug();

protected:
    entt::registry _registry;

    // manage scene
    bool _isChangeScene = false;
    std::string _nextScene;

    // debugging
    bool _isDebug = false;

    void renderText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int x, int y, int width, int height,
                    SDL_Color color);
};


#endif //CBIT2D_SCENE_H
