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

#include <functional>
#include "Type.h"
#include <glm/glm.hpp>

struct TransformComponent {
    int x, y, width, height;
    int velocityX = 0;
    int velocityY = 0;
    int scale = 1;
    glm::vec2 position = glm::vec2(x, y);
    glm::vec2 velocity = glm::vec2(velocityX, velocityY);

    TransformComponent(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
};

struct ColliderComponent {
    int x, y, width, height, offsetX, offsetY;
};

struct SpriteComponent {
    std::string textureName;
    int x, y, width, height;
};

struct TextComponent {
    std::string text;
    std::string fontName;
    int size;
    Color textColor;
};

struct ButtonComponent {
    std::function<void()> onClick;
    bool isHover;
};


#endif //CBIT2D_COMPONENTS_H
