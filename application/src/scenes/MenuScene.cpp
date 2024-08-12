/**
 * @file MenuScene.cpp
 * @brief Implementation file for the MenuScene class.
 *
 * This file contains the implementation of the MenuScene class which is responsible for managing the menu scene in the game.
 * The MenuScene class is responsible for setting up the menu scene, updating the menu scene, and handling input for the menu scene.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-21
 */

#include "MenuScene.h"

MenuScene::MenuScene() : Scene() {
}

MenuScene::~MenuScene() = default;

void MenuScene::setup() {
    Scene::setup();
//    toggleDebug();
    // set background color to yellow
    setBackgroundColour(Color{55, 138, 249, 255});
    // set background music
    playBGM("easy-cheesy-bgm");

    auto _titleText = _ecs.createGameObject("titleText");
    _titleText.addComponent<TextComponent>("Sokoban", "JetBrainsMono-Regular", 32, 255, 255, 255, 255);
    _titleText.addComponent<TransformComponent>(356, 194, 100, 50);

    auto _playButton = _ecs.createGameObject("playButton");
    _playButton.addComponent<TransformComponent>(339, 258, 150, 50);
    _playButton.addComponent<TextComponent>("Play", "Kenney_Future", 32, Color{255, 255, 255, 255});
    _playButton.addComponent<SpriteComponent>("button_rectangle_depth_flat", 0, 0, 384, 128, 1);
    _playButton.addComponent<ButtonComponent>([this]() {
        LOG_INFO("Changing scene to PlayScene");
        playSFX("click-a");
        changeScene("PlayScene");
    });

//    _ecs.registry.emplace<ClickableComponent>(_playButton);
}

void MenuScene::update(float deltaTime, Input &input) {
    Scene::update(deltaTime, input);

//    auto clickableView = _ecs.registry.view<ClickableComponent>();
//
//    for (auto entity : clickableView) {
//        auto &clickableComponent = clickableView.get<ClickableComponent>(entity);
//        if (clickableComponent.isClicked) {
//            LOG_INFO("Changing scene to PlayScene");
//            playSFX("click-a");
//            changeScene("PlayScene");
//        }
//    }

}
