//
// Created by User on 22/7/2024.
//

#ifndef CBIT2D_PLAYSCENE_H
#define CBIT2D_PLAYSCENE_H


#include "../../../src/core/Scene.h"

class PlayScene : public Scene {
public:
    PlayScene();

    ~PlayScene() override;

    void setup() override;

    void update() override;

    void render(SDL_Renderer *renderer) override;

    void handleInput(SDL_Event event) override;

    void cleanup() override;

};


#endif //CBIT2D_PLAYSCENE_H
