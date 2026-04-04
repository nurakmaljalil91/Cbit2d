/**
 * @file    play_scene.hpp
 * @brief   Header file for the sandbox play scene.
 * @details This file declares the PlayScene class used by the sandbox
 *          application to exercise engine scene and ECS behavior.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#pragma once

#include "cbit/core/scene.hpp"

namespace sandbox::scenes {

/**
 * @brief Example gameplay scene used by the sandbox application.
 */
class PlayScene final : public cbit2d::core::Scene {
public:
    /**
     * @brief Initializes the play scene.
     */
    void initialize() override;

    /**
     * @brief Updates the play scene once per frame.
     * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
     */
    void update(float deltaTimeSeconds) override;

private:
    float _deltaTimeSeconds = 0.0F;
};

} // namespace sandbox::scenes
