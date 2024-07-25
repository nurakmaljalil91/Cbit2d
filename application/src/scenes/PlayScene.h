/**
 * @file PlayScene.h
 * @brief Header file for the PlayScene class.
 *
 * This file contains the definition of the PlayScene class which is responsible for managing the play scene in the game.
 * The PlayScene class is responsible for setting up the play scene, updating the play scene, and handling input for the play scene.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */

#ifndef CBIT2D_PLAYSCENE_H
#define CBIT2D_PLAYSCENE_H


#include "../../../src/core/Scene.h"

class PlayScene : public Scene {
public:
    PlayScene();

    ~PlayScene() override;

    void setup() override;

    void update() override;

    void handleInput(SDL_Event event) override;
};


#endif //CBIT2D_PLAYSCENE_H
