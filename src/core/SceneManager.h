//
// Created by User on 21/7/2024.
//

#ifndef CBIT2D_SCENEMANAGER_H
#define CBIT2D_SCENEMANAGER_H

#include "Scene.h"
#include <memory>
#include <unordered_map>
#include <string>


class SceneManager {
public:
    SceneManager();

    ~SceneManager();

    void init();

    void update();

    void render();

    void handleInput();

    void cleanup();

    void addScene(const std::string& name, std::shared_ptr<Scene> scene);
    void switchTo(const std::string& name);

    void changeScene();
private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> currentScene;
};


#endif //CBIT2D_SCENEMANAGER_H
