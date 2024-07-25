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

struct TransformComponent {
    int x;
    int y;
    int width;
    int height;
};

struct SpriteComponent {
    SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
};

struct TextComponent {
    std::string text;
    TTF_Font* font;
    SDL_Color color;
};

struct BgmComponent {
    Mix_Music *bgm;
};


#endif //CBIT2D_COMPONENTS_H
