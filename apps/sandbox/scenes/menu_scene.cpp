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
#include "cbit/ui/ui_context.hpp"

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
    auto& root = getUi().root();
    root.layout().horizontalAlignment = cbit::ui::UiAlignment::Center;
    root.layout().verticalAlignment = cbit::ui::UiAlignment::Center;
    root.layout().gap = 36.0F;

    cbit::ui::UiTextStyle titleStyle;
    titleStyle.fontPath = "apps/sandbox/assets/fonts/Kenney_Future.ttf";
    titleStyle.fontSize = 64.0F;
    titleStyle.color = {255, 245, 196, 255};
    root.addText("Sokoban", titleStyle);

    cbit::ui::UiButtonStyle buttonStyle;
    buttonStyle.backgroundColor = {20, 28, 48, 255};
    buttonStyle.hoverColor = {34, 48, 83, 255};
    buttonStyle.pressedColor = {15, 21, 36, 255};
    buttonStyle.borderColor = {243, 200, 88, 255};
    auto& button = root.addButton("Play", _onPlaySelected, buttonStyle);
    button.layout().width = cbit::ui::UiLength::pixels(240.0F);
    button.layout().height = cbit::ui::UiLength::pixels(76.0F);
    cbit::ui::UiTextStyle buttonTextStyle;
    buttonTextStyle.fontPath = "apps/sandbox/assets/fonts/Kenney_Future_Narrow.ttf";
    buttonTextStyle.fontSize = 32.0F;
    buttonTextStyle.color = {255, 255, 255, 255};
    button.setTextStyle(buttonTextStyle);

    cbit2d::core::Logger::info("MenuScene initialized");
}

/**
 * @brief Updates the menu scene once per frame.
 * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
 */
void MenuScene::update(float deltaTimeSeconds)
{
    world.update(deltaTimeSeconds);
    getUi().update();
}

} // namespace sandbox::scenes
