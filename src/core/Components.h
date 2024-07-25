/**
 * @file Components.h
 * @brief Header file for the Components struct.
 *
 * This file contains the definition of the Components struct which is responsible for storing the components of an entity.
 * The Components struct is responsible for storing the components of an entity.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-25
 */


#ifndef CBIT2D_COMPONENTS_H
#define CBIT2D_COMPONENTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <functional>

struct TransformComponent {
    int x, y, width, height;
};

struct SpriteComponent {
    std::string textureName;
    int x, y, width, height;
};

struct TextComponent {
    std::string text;
    std::string fontName;
    int size;
    int r, g, b, a;
};

struct BgmComponent {
    Mix_Music *bgm;
};

struct ButtonComponent {
    std::function<void()> onClick;
    bool isHover;
};


#endif //CBIT2D_COMPONENTS_H
