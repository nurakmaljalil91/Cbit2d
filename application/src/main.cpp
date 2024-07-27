/**
 * @file main.cpp
 * @brief Main file for the SDL2 application with logging using spdlog.
 *
 * This file contains the main function for an SDL2 application that sets up
 * a _window and logs events using the Logger class defined in Logger.h.
 *
 * The application initializes SDL2, creates a _window, and enters a main loop
 * where it handles events until the user requests to quit.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-20
 */
#include "../../src/core/Application.h"
#include "scenes/MenuScene.h"
#include "scenes/PlayScene.h"

int main(int argc, char *argv[]) {

    Application app("CBit 2D Application", 832, 640, false);

    if (!app.init()) {
        return -1;
    }

    app.getSceneManager().addScene("MenuScene", std::make_shared<MenuScene>());
    app.getSceneManager().addScene("PlayScene", std::make_shared<PlayScene>());
    app.getSceneManager().setActiveScene("MenuScene");

    app.run();

    return 0;
}