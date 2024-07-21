
#ifndef CBIT2D_SCENE_H
#define CBIT2D_SCENE_H

#include <entt/entt.hpp>
#include <SDL2/SDL.h>

class Scene {
public:
    Scene();

    virtual ~Scene();

    virtual void init();

    virtual void update();

    virtual void render();

    virtual void handleInput();

    virtual void cleanup();

protected:
    entt::registry registry;
};


#endif //CBIT2D_SCENE_H
