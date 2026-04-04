/**
 * @file    menu_scene.hpp
 * @brief   Header file for the sandbox menu scene.
 * @details This file declares the MenuScene class used by the sandbox
 *          application as a placeholder for future menu-oriented scene logic.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#pragma once

#include <functional>

#include "cbit/core/scene.hpp"

namespace sandbox::scenes {

/**
 * @brief Menu scene used by the sandbox application.
 */
class MenuScene final : public cbit2d::core::Scene {
public:
    /**
     * @brief Creates a menu scene with a play action callback.
     * @param onPlaySelected Callback invoked when the play button is activated.
     */
    explicit MenuScene(std::function<void()> onPlaySelected);

    /**
     * @brief Initializes the menu scene.
     */
    void initialize() override;

    /**
     * @brief Updates the menu scene once per frame.
     * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
     */
    void update(float deltaTimeSeconds) override;

private:
    std::function<void()> _onPlaySelected;
};

} // namespace sandbox::scenes
