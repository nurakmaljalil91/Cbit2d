//
// Created by User on 22/7/2024.
//

#ifndef CBIT2D_PLAYSCENE_H
#define CBIT2D_PLAYSCENE_H


#include "../../../src/core/Scene.h"

class PlayScene : public Scene {
public:
    PlayScene();

    ~PlayScene();

    void setup(SDL_Renderer *renderer);

    void update();

    void render(SDL_Renderer *renderer);

    void handleInput(SDL_Event event);

    void cleanup();

};


#endif //CBIT2D_PLAYSCENE_H
