//
// Created by User on 21/7/2024.
//

#ifndef CBIT2D_CUSTOMSCENE_H
#define CBIT2D_CUSTOMSCENE_H

#include "../../src/core/Scene.h"

class CustomScene: public Scene {
public:
    CustomScene();
    ~CustomScene();

    void init();
    void update();
    void render();
    void handleInput(SDL_Event event);
    void cleanup();
};


#endif //CBIT2D_CUSTOMSCENE_H
