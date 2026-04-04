/**
 * @file    menu_scene.cpp
 * @brief   Source file for the sandbox menu scene.
 * @details This file defines the MenuScene class used by the sandbox
 *          application to render a title and clickable play button that
 *          transitions into the play scene.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#include "menu_scene.hpp"

#include <utility>

#include "cbit/core/logger.hpp"
#include "cbit/ecs/components.hpp"

namespace sandbox::scenes {

/**
 * @brief Creates a menu scene with a play action callback.
 * @param onPlaySelected Callback invoked when the play button is activated.
 */
MenuScene::MenuScene(std::function<void()> onPlaySelected)
    : _onPlaySelected(std::move(onPlaySelected))
{
}

/**
 * @brief Initializes the menu scene.
 */
void MenuScene::initialize()
{
    auto title = world.addGameObject("MenuTitle");
    auto& titleTransform = title.getComponent<cbit::ecs::TransformComponent>();
    titleTransform.position = {400.0F, 180.0F};

    auto& titleText = title.addComponent<cbit::ecs::TextComponent>();
    titleText.content = "Sokoban";
    titleText.fontPath = "apps/sandbox/assets/fonts/Kenney_Future.ttf";
    titleText.fontSize = 64.0F;
    titleText.color = {255, 245, 196, 255};

    auto playButton = world.addGameObject("PlayButton");
    auto& buttonTransform = playButton.getComponent<cbit::ecs::TransformComponent>();
    buttonTransform.position = {400.0F, 320.0F};

    auto& button = playButton.addComponent<cbit::ecs::ButtonComponent>();
    button.size = {240.0F, 76.0F};
    button.backgroundColor = {20, 28, 48, 255};
    button.hoverColor = {34, 48, 83, 255};
    button.pressedColor = {15, 21, 36, 255};
    button.borderColor = {243, 200, 88, 255};
    button.onClick = _onPlaySelected;

    auto& buttonText = playButton.addComponent<cbit::ecs::TextComponent>();
    buttonText.content = "Play";
    buttonText.fontPath = "apps/sandbox/assets/fonts/Kenney_Future_Narrow.ttf";
    buttonText.fontSize = 32.0F;
    buttonText.color = {255, 255, 255, 255};

    cbit2d::core::Logger::info("MenuScene initialized");
}

/**
 * @brief Updates the menu scene once per frame.
 * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
 */
void MenuScene::update(float deltaTimeSeconds)
{
    world.update(deltaTimeSeconds);
}

} // namespace sandbox::scenes
