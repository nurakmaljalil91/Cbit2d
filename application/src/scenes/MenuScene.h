//
// Created by User on 22/7/2024.
//

#ifndef CBIT2D_MENUSCENE_H
#define CBIT2D_MENUSCENE_H


#include "../../../src/core/Scene.h"

class MenuScene : public Scene {
public:
    MenuScene();

    ~MenuScene() override;

    void setup() override;

    void update() override;

    void render(SDL_Renderer *renderer) override;

    void handleInput(SDL_Event event) override;

    void cleanup();
private:
    TTF_Font* _defaultFont{};
    entt::entity _titleText;
    entt::entity _playButton;
    entt::entity _bgm;
};


#endif //CBIT2D_MENUSCENE_H
