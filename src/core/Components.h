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
#include <utility>

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
    std::string tag;
    int x, y, width, height;

    ColliderComponent(std::string tag, int width, int height) : tag(std::move(tag)), x(0), y(0), width(width),
                                                                height(height) {}
};

struct SpriteComponent {
    std::string textureName;
    int x, y, width, height;

    SpriteComponent() = default;

    SpriteComponent(std::string textureName, int x, int y, int width, int height) :
            textureName(std::move(textureName)), x(x), y(y), width(width), height(height) {}
};

struct MultipleSpriteComponent {
    std::unordered_map<std::string, SpriteComponent> sprites;
    std::string currentSprite;
};

struct Animation {
    int x, y;
    int frameCount;
    float frameTime;
    int startFrame;
};

struct AnimatedSpriteComponent {
    std::string textureName;
    int width, height;        // Dimensions of each frame
    int currentFrame;
    float currentTime;       // Time accumulator for the current frame
    std::unordered_map<std::string, Animation> animations; // Different animations
    std::string currentAnimation; // The current animation name

    AnimatedSpriteComponent(std::string textureName, int width, int height) :
            textureName(textureName), width(width), height(height),
            currentFrame(0), currentTime(0) {}

    void addAnimation(const std::string &name, int x, int y, int frameCount, float frameTime) {
        animations[name] = Animation{x, y, frameCount, frameTime, currentFrame};
    }

    void playAnimation(const std::string &name) {
        if (currentAnimation != name) {
            currentAnimation = name;
            currentFrame = animations[name].startFrame;
            currentTime = 0;
        }
    }

    const Animation &getCurrentAnimation() {
        return animations[currentAnimation];
    }
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

struct DraggableComponent {
    bool isDragging;
    glm::vec2 offset;
};

struct ClickableComponent {
    bool isClicked;
    bool isHover;
};

#endif //CBIT2D_COMPONENTS_H
