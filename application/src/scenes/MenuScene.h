//
// Created by User on 22/7/2024.
//

#ifndef CBIT2D_MENUSCENE_H
#define CBIT2D_MENUSCENE_H


#include "../../../src/core/Scene.h"

class MenuScene : public Scene {
public:
    MenuScene();

    ~MenuScene();

    void setup(SDL_Renderer *renderer);

    void update();

    void render(SDL_Renderer *renderer);

    void handleInput(SDL_Event event);

    void cleanup();
private:
    TTF_Font* _defaultFont;
    entt::entity _titleText;
};


#endif //CBIT2D_MENUSCENE_H
