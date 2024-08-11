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

    auto _titleText = _ecs.registry.create();
    _ecs.registry.emplace<TextComponent>(_titleText, "Sokoban", "JetBrainsMono-Regular", 32, 255, 255, 255, 255);
    _ecs.registry.emplace<TransformComponent>(_titleText, 356, 194, 100, 50);

    auto _playButton = _ecs.registry.create();
    _ecs.registry.emplace<TransformComponent>(_playButton, 339, 258, 150, 50);
    _ecs.registry.emplace<TextComponent>(_playButton, "Play", "Kenney_Future", 32, Color{255, 255, 255, 255});
    _ecs.registry.emplace<SpriteComponent>(_playButton, "button_rectangle_depth_flat", 0, 0, 384, 128, 1);
    _ecs.registry.emplace<ButtonComponent>(_playButton, [this]() {
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
