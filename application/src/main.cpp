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
#include "scenes/CustomScene.h"

int main(int argc, char* argv[]) {
    Application app("Cbit 2D Game Engine", 800, 600);
    if (!app.init()) {
        return 1;
    }

    app.getSceneManager().addScene("CustomScene",std::make_shared<CustomScene>());
    app.getSceneManager().setCurrentScene("CustomScene");

    app.run();

    return 0;
}