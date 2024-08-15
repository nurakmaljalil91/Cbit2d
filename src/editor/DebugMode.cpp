/**
 * @file Editor.cpp
 * @brief Implementation file for the Editor class.
 *
 * This file contains the implementation of the Editor class which is responsible for managing the editor in the game.
 * The Editor class is responsible for managing the editor in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-08-13
 */
#include "DebugMode.h"

#ifdef ENABLE_EDITOR


DebugMode::DebugMode() {

}

DebugMode::~DebugMode() {

}

void DebugMode::setup(SDL_Window *window, SDL_Renderer *renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
}


void DebugMode::handleInput(SDL_Event &event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void DebugMode::update(float deltaTime, SceneManager &sceneManager) {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (_showDemoWindow) {
        ImGui::ShowDemoWindow(&_showDemoWindow);
    }

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    if (_showSimpleWindow) {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin(
                "Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &_showDemoWindow);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &_showAnotherWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float *) &_clearColor); // Edit 3 floats representing a color

        if (ImGui::Button(
                "Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGuiIO &io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        // Add a button to toggle debug mode
        if (ImGui::Button("Toggle Debug Mode")) {
            sceneManager.getActiveScene().toggleDebugMode();
        }
        ImGui::End();
    }

    if (_showAnotherWindow) {
        ImGui::Begin("Another Window",
                     &_showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            _showAnotherWindow = false;
        ImGui::End();
    }

    renderEntitiesPanel(sceneManager);

    renderScenesPanel(sceneManager);

    ImGui::Render();
}

void DebugMode::render(SDL_Renderer *renderer) {
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}

void DebugMode::cleanup() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void DebugMode::renderEntitiesPanel(SceneManager &sceneManager) {
    auto &scenes = sceneManager.getActiveScene();

    auto &ecs = scenes.getECS();

    ImGui::Begin("Game Objects");

    auto view = ecs.getAllEntitiesWith<TransformComponent, TagComponent, IdComponent>();

    for (auto entity: view) {
        auto &tag = view.get<TagComponent>(entity);
        auto &id = view.get<IdComponent>(entity);
        auto &transform = view.get<TransformComponent>(entity);

        // Display the entity's tag
        if (ImGui::TreeNode(tag.tag.c_str())) {

            // Display the entity's ID
            ImGui::Text("ID: %d", id.uuid.data());

            // Display and modify the position
            ImGui::Text("Position:");
            ImGui::DragFloat("X", &transform.position.x, 1.0f);
            ImGui::DragFloat("Y", &transform.position.y, 1.0f);

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void DebugMode::renderScenesPanel(SceneManager &sceneManager) {
    ImGui::Begin("Scenes");

    // Retrieve the list of scenes from the SceneManager
    const auto &scenes = sceneManager.getScenes();

    // Iterate through the list of scenes
    for (const auto &scene: scenes) {
        // Display the scene's name
        if (ImGui::TreeNode(scene.first.c_str())) {
            // Add a button to switch to the scene
            if (ImGui::Button("Switch to Scene")) {
                sceneManager.setActiveScene(scene.first);
            }
            // toggle debug mode
            ImGui::Checkbox("Toggle Debug Mode", &scene.second->isDebug);
            ImGui::TreePop();
        }
    }

    ImGui::End();
}

#endif // ENABLE_EDITOR