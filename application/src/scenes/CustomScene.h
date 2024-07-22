//
// Created by User on 21/7/2024.
//

#ifndef CBIT2D_CUSTOMSCENE_H
#define CBIT2D_CUSTOMSCENE_H

#include "../../src/core/Scene.h"

class CustomScene : public Scene {
public:
    CustomScene();

    ~CustomScene();

    void setup(SDL_Renderer *renderer);

    void update();

    void handleInput(SDL_Event event);

    void cleanup();

private:
    entt::entity _image;
};


#endif //CBIT2D_CUSTOMSCENE_H
