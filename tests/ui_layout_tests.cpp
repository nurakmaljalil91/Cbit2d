#include <cmath>
#include <cstdlib>
#include <iostream>

#include <SDL3/SDL.h>

#include "cbit/core/input.hpp"
#include "cbit/ui/ui_context.hpp"

namespace {

void expectNear(const float actual, const float expected, const char* label)
{
    if (std::fabs(actual - expected) > 0.01F) {
        std::cerr << label << ": expected " << expected << ", got " << actual << '\n';
        std::exit(EXIT_FAILURE);
    }
}

void moveMouse(const float x, const float y)
{
    SDL_Event event {};
    event.type = SDL_EVENT_MOUSE_MOTION;
    event.motion.x = x;
    event.motion.y = y;
    cbit2d::core::Input::processEvent(event);
}

void clickMouse(const float x, const float y)
{
    cbit2d::core::Input::beginFrame();
    SDL_Event event {};
    event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
    event.button.button = SDL_BUTTON_LEFT;
    event.button.x = x;
    event.button.y = y;
    cbit2d::core::Input::processEvent(event);
}

} // namespace

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) return EXIT_FAILURE;

    cbit::ui::UiContext centered;
    centered.setViewport({200.0F, 100.0F});
    centered.root().layout().horizontalAlignment = cbit::ui::UiAlignment::Center;
    centered.root().layout().verticalAlignment = cbit::ui::UiAlignment::Center;
    auto& panel = centered.root().addPanel();
    panel.layout().width = cbit::ui::UiLength::pixels(40.0F);
    panel.layout().height = cbit::ui::UiLength::pixels(20.0F);
    centered.update();
    expectNear(panel.rect().x, 80.0F, "centered x");
    expectNear(panel.rect().y, 40.0F, "centered y");
    centered.setViewport({300.0F, 200.0F});
    centered.update();
    expectNear(panel.rect().x, 130.0F, "resized centered x");
    expectNear(panel.rect().y, 90.0F, "resized centered y");

    cbit::ui::UiContext rowLayout;
    rowLayout.setViewport({100.0F, 40.0F});
    auto& row = rowLayout.root().addRow();
    row.layout().width = cbit::ui::UiLength::percent(100.0F);
    row.layout().height = cbit::ui::UiLength::pixels(40.0F);
    auto& fixed = row.addPanel();
    fixed.layout().width = cbit::ui::UiLength::pixels(20.0F);
    fixed.layout().height = cbit::ui::UiLength::pixels(40.0F);
    auto& flexible = row.addPanel();
    flexible.layout().width = cbit::ui::UiLength::grow();
    flexible.layout().height = cbit::ui::UiLength::pixels(40.0F);
    rowLayout.update();
    expectNear(fixed.rect().width, 20.0F, "fixed width");
    expectNear(flexible.rect().x, 20.0F, "flexible x");
    expectNear(flexible.rect().width, 80.0F, "flexible width");

    cbit::ui::UiContext columnLayout;
    columnLayout.setViewport({40.0F, 100.0F});
    auto& columnFixed = columnLayout.root().addPanel();
    columnFixed.layout().width = cbit::ui::UiLength::pixels(40.0F);
    columnFixed.layout().height = cbit::ui::UiLength::pixels(20.0F);
    auto& columnFlexible = columnLayout.root().addPanel();
    columnFlexible.layout().width = cbit::ui::UiLength::pixels(40.0F);
    columnFlexible.layout().height = cbit::ui::UiLength::grow();
    columnLayout.update();
    expectNear(columnFlexible.rect().y, 20.0F, "column flexible y");
    expectNear(columnFlexible.rect().height, 80.0F, "column flexible height");

    bool clicked = false;
    cbit::ui::UiContext clipped;
    clipped.setViewport({100.0F, 100.0F});
    auto& clipPanel = clipped.root().addPanel();
    clipPanel.layout().width = cbit::ui::UiLength::pixels(50.0F);
    clipPanel.layout().height = cbit::ui::UiLength::pixels(50.0F);
    clipPanel.layout().clipChildren = true;
    auto& button = clipPanel.addButton("", [&clicked] { clicked = true; });
    button.layout().width = cbit::ui::UiLength::pixels(60.0F);
    button.layout().height = cbit::ui::UiLength::pixels(30.0F);
    clipped.update();
    moveMouse(55.0F, 15.0F);
    clickMouse(55.0F, 15.0F);
    clipped.update();
    if (clicked) return EXIT_FAILURE;
    SDL_Event mouseUp {};
    mouseUp.type = SDL_EVENT_MOUSE_BUTTON_UP;
    mouseUp.button.button = SDL_BUTTON_LEFT;
    cbit2d::core::Input::processEvent(mouseUp);
    moveMouse(25.0F, 15.0F);
    clickMouse(25.0F, 15.0F);
    clipped.update();
    if (!clicked) return EXIT_FAILURE;

    SDL_Quit();
    return EXIT_SUCCESS;
}
