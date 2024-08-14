/**
 * @file Editor.h
 * @brief Header file for the Editor class.
 * This file contains the declaration of the Editor class which is responsible for managing the editor in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-08-13
 */
#ifdef ENABLE_EDITOR

#ifndef CBIT2D_EDITOR_H
#define CBIT2D_EDITOR_H

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>
#include <SDL2/SDL.h>

class Editor {
public:
    Editor();

    ~Editor();

    void setup(SDL_Window *window, SDL_Renderer *renderer);

    void update(float deltaTime);

    void render(SDL_Renderer *renderer);

    void cleanup();

    void handleInput(SDL_Event &event);

    void toggleEditor();

    bool isEditorActive() const;

private:
    bool _showDemoWindow = true;
    bool _showAnotherWindow = false;
    ImVec4 _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool _editorActive = false;
};

#endif //CBIT2D_EDITOR_H

#endif // ENABLE_EDITOR
