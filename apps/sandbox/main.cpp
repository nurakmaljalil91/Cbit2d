/**
 * @file    main.cpp
 * @brief   Entry point for the sandbox application.
 * @details This file contains the standard C++ entry point which boots the
 *          engine application, registers the sandbox play scene, and transfers
 *          lifecycle control to the `cbit2d::core::Application` class.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#include <memory>

#include "cbit/core/application.hpp"
#include "scenes/menu_scene.hpp"
#include "scenes/play_scene.hpp"

/**
 * @brief Starts the sandbox application.
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument array.
 * @return Process exit code.
 */
int main(int argc, char* argv[])
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    cbit2d::core::Application application;
    application.getSceneManager().addScene(
        "MenuScene",
        std::make_shared<sandbox::scenes::MenuScene>([&application]() {
            application.getSceneManager().setActiveScene("PlayScene");
        })
    );
    application.getSceneManager().addScene("PlayScene", std::make_shared<sandbox::scenes::PlayScene>());
    return application.run("Hello World", 800, 600);
}
