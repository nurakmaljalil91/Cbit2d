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
#include "Input.h"
#include "EntityComponentSystem.h"
#include "TileMap.h"

class Scene {
public:
    Scene();

    virtual ~Scene();

    // scene lifecycle
    virtual void setup();

    virtual void update(float deltaTime, Input &input);

    void render(SDL_Renderer *renderer);

    void cleanup();

    // handle scenes management (use by SceneManager)
    [[nodiscard]] bool switchScene() const;

    void changeScene(const std::string &name);

    std::string getNextScene();

protected:
    EntityComponentSystem _ecs;

    // manage scene
    bool _isChangeScene = false;
    std::string _nextScene;

    // debugging
    bool _isDebug = false;

    void toggleDebug();

    // background color
    Color _backgroundColor = {0, 0, 0, 255};
    bool _isBackgroundColorSet = false;

    void setBackgroundColour(Color color);

    // background music
    Mix_Music *_bgm = nullptr;

    void playBGM(const std::string &name);

    static void stopBGM();

    static void playSFX(const std::string &name);

    // manage tile map
    TileMap _tileMap;
};


#endif //CBIT2D_SCENE_H
