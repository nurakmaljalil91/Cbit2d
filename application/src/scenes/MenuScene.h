/**
 * @file MenuScene.h
 * @brief Header file for the MenuScene class.
 *
 * This file contains the definition of the MenuScene class which is responsible for managing the menu scene in the game.
 * The MenuScene class is responsible for setting up the menu scene, updating the menu scene, and handling input for the menu scene.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */

#ifndef CBIT2D_MENUSCENE_H
#define CBIT2D_MENUSCENE_H


#include "../../../src/core/Scene.h"

class MenuScene : public Scene {
public:
    MenuScene();

    ~MenuScene() override;

    void setup() override;

    void update(Input &input) override;
};


#endif //CBIT2D_MENUSCENE_H
