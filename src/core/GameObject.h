/**
 * @file Entity.h
 * @brief Entity class header file
 *
 * This file contains the definition of the Entity class which is responsible for managing the entities in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-08-12
 */

#ifndef CBIT2D_GAMEOBJECT_H
#define CBIT2D_GAMEOBJECT_H

#include <entt/entt.hpp>
#include "EntityComponentSystem.h"

class GameObject {
public:
    GameObject() = default;

    ~GameObject() = default;

    GameObject(entt::entity entity, EntityComponentSystem *ecs) : _entity(entity), _ecs(ecs) {}

    template<typename T, typename... Args>
    T &addComponent(Args &&... args) {
        return _ecs->_registry.emplace<T>(_entity, std::forward<Args>(args)...);
    }

    template<typename T>
    T &getComponent() {
        return _ecs->_registry.get<T>(_entity);
    }

    template<typename T>
    bool hasComponent() {
        return _ecs->_registry.any_of<T>(_entity);
    }

    template<typename T>
    void removeComponent() {
        _ecs->_registry.remove<T>(_entity);
    }

    void destroy() {
        _ecs->_registry.destroy(_entity);
    }

    [[nodiscard]] entt::entity getEntity() const {
        return _entity;
    }

    void setEntity(entt::entity entity) {
        _entity = entity;
    }

    void setECS(EntityComponentSystem *ecs) {
        _ecs = ecs;
    }

    [[nodiscard]] EntityComponentSystem *getECS() const {
        return _ecs;
    }

    bool operator==(const GameObject &other) const {
        return _entity == other._entity;
    }

private:
    entt::entity _entity{entt::null};
    EntityComponentSystem *_ecs = nullptr;
};


#endif //CBIT2D_GAMEOBJECT_H
