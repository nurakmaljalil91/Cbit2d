//
// Created by User on 22/7/2024.
//

#ifndef CBIT2D_COMPONENTS_H
#define CBIT2D_COMPONENTS_H

#include <SDL2/SDL.h>

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
    SDL_Color color;
    TTF_Font* font;
    SDL_Texture* texture;
    int width, height;
};



#endif //CBIT2D_COMPONENTS_H
