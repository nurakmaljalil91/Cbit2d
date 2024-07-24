/**
 * @file SceneManager.h
 * @brief Header file for the SceneManager class.
 *
 * This file contains the definition of the SceneManager class which is responsible for managing the scenes in the game.
 * The SceneManager class is responsible for adding, switching, and updating the scenes in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */

#ifndef CBIT2D_SCENEMANAGER_H
#define CBIT2D_SCENEMANAGER_H

#include "Scene.h"
#include <memory>
#include <unordered_map>
#include <string>


class SceneManager {
public:
    static SceneManager &getInstance() {
        static SceneManager instance;
        return instance;
    }

    ~SceneManager();
    
    void update();

    void render(SDL_Renderer *renderer);

    void handleInput(SDL_Event event);

    void cleanup();

    void addScene(const std::string &name, std::shared_ptr<Scene> scene);

    void setActiveScene(const std::string &name);

private:
    SceneManager();

    std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;
    std::shared_ptr<Scene> _currentScene;
};


#endif //CBIT2D_SCENEMANAGER_H
