/**
 * @file    play_scene.cpp
 * @brief   Source file for the sandbox play scene.
 * @details This file defines the PlayScene class used by the sandbox
 *          application to validate scene setup, ECS updates, and input-driven
 *          movement behavior.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#include "play_scene.hpp"

#include <filesystem>

#include "cbit/assets/tile_map.hpp"
#include "cbit/core/input.hpp"
#include "cbit/core/logger.hpp"
#include "cbit/ecs/components.hpp"

namespace sandbox::scenes {

namespace {

/**
 * @brief Stores an axis-aligned rectangle used for collision checks.
 */
struct Rect {
    float left = 0.0F;
    float top = 0.0F;
    float right = 0.0F;
    float bottom = 0.0F;
};

/**
 * @brief Builds a world-space collider rectangle from transform and collider data.
 * @param transform Transform owning the collider position.
 * @param collider Collider definition in local space.
 * @return Axis-aligned rectangle in world space.
 */
Rect makeRect(const cbit::ecs::TransformComponent& transform, const cbit::ecs::ColliderComponent& collider)
{
    const glm::vec2 center = transform.position + collider.offset;
    const glm::vec2 halfSize = collider.size * 0.5F;

    return {
        center.x - halfSize.x,
        center.y - halfSize.y,
        center.x + halfSize.x,
        center.y + halfSize.y
    };
}

/**
 * @brief Returns whether two axis-aligned rectangles overlap.
 * @param left Left-hand rectangle.
 * @param right Right-hand rectangle.
 * @return `true` when the rectangles overlap, otherwise `false`.
 */
bool intersects(const Rect& left, const Rect& right)
{
    return left.left < right.right
        && left.right > right.left
        && left.top < right.bottom
        && left.bottom > right.top;
}

/**
 * @brief Returns whether moving a collider to a target position hits a static collider.
 * @param ecs ECS world containing collidable entities.
 * @param movingEntity Entity being moved.
 * @param targetPosition Candidate target position for the moving entity.
 * @return `true` when the moved collider would overlap a static solid collider.
 */
bool collidesWithStaticWorld(
    cbit::ecs::EntityComponentSystem& ecs,
    const entt::entity movingEntity,
    const glm::vec2& targetPosition
)
{
    auto& registry = ecs.getRegistry();
    if (!registry.all_of<cbit::ecs::TransformComponent, cbit::ecs::ColliderComponent>(movingEntity)) {
        return false;
    }

    const auto& movingCollider = registry.get<cbit::ecs::ColliderComponent>(movingEntity);
    const auto& movingTransform = registry.get<cbit::ecs::TransformComponent>(movingEntity);
    const auto movingRect = makeRect(
        cbit::ecs::TransformComponent {targetPosition, movingTransform.scale, movingTransform.rotation},
        movingCollider
    );

    auto colliderView = registry.view<const cbit::ecs::TransformComponent, const cbit::ecs::ColliderComponent>();
    for (const auto entity : colliderView) {
        if (entity == movingEntity) {
            continue;
        }

        const auto& collider = colliderView.get<const cbit::ecs::ColliderComponent>(entity);
        if (collider.isTrigger || !collider.isStatic) {
            continue;
        }

        const auto& transform = colliderView.get<const cbit::ecs::TransformComponent>(entity);
        if (intersects(movingRect, makeRect(transform, collider))) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Resolves the tilesheet path used by the sandbox map.
 * @param map Loaded tile map data.
 * @param tileset Tileset referenced by a spawned tile.
 * @return Asset path consumable by the existing sprite renderer.
 */
std::string resolveTilesetAssetPath(
    const cbit2d::assets::TileMap& map,
    const cbit2d::assets::TileMapTileset& tileset
)
{
    const std::filesystem::path resolvedPath = map.resolveTilesetImagePath(tileset);
    if (std::filesystem::exists(resolvedPath)) {
        return resolvedPath.string();
    }

    return "apps/sandbox/assets/images/" + std::filesystem::path(tileset.imagePath).filename().string();
}

/**
 * @brief Spawns tile entities from a loaded tile map.
 * @param world Scene ECS world receiving the generated tiles.
 * @param map Loaded tile map data.
 */
void spawnMapTiles(cbit::ecs::EntityComponentSystem& world, const cbit2d::assets::TileMap& map)
{
    const float tileWidth = static_cast<float>(map.getTileWidth());
    const float tileHeight = static_cast<float>(map.getTileHeight());

    for (const auto& layer : map.getLayers()) {
        if (!layer.visible) {
            continue;
        }

        for (std::uint32_t row = 0; row < layer.height; ++row) {
            for (std::uint32_t column = 0; column < layer.width; ++column) {
                const std::size_t tileIndex = static_cast<std::size_t>(row * layer.width + column);
                if (tileIndex >= layer.data.size()) {
                    continue;
                }

                const std::uint32_t gid = layer.data[tileIndex];
                if (gid == 0) {
                    continue;
                }

                const auto* tileset = map.findTilesetForGid(gid);
                if (tileset == nullptr || tileset->columns == 0) {
                    continue;
                }

                const std::uint32_t localTileId = gid - tileset->firstGid;
                const std::uint32_t sourceColumn = localTileId % tileset->columns;
                const std::uint32_t sourceRow = localTileId / tileset->columns;

                auto tile = world.addGameObject(layer.name + "_" + std::to_string(column) + "_" + std::to_string(row));
                auto& transform = tile.getComponent<cbit::ecs::TransformComponent>();
                transform.position = {
                    (static_cast<float>(column) * tileWidth) + (tileWidth * 0.5F),
                    (static_cast<float>(row) * tileHeight) + (tileHeight * 0.5F)
                };

                auto& sprite = tile.addComponent<cbit::ecs::SpriteComponent>();
                sprite.assetPath = resolveTilesetAssetPath(map, *tileset);
                sprite.textureSize = {
                    static_cast<float>(tileset->imageWidth),
                    static_cast<float>(tileset->imageHeight)
                };
                sprite.sourcePosition = {
                    static_cast<float>(sourceColumn * tileset->tileWidth),
                    static_cast<float>(sourceRow * tileset->tileHeight)
                };
                sprite.sourceSize = {
                    static_cast<float>(tileset->tileWidth),
                    static_cast<float>(tileset->tileHeight)
                };
                sprite.size = sprite.sourceSize;
                sprite.renderOrder = layer.name == "Wall" ? 1 : 0;

                if (layer.name == "Wall") {
                    auto& collider = tile.addComponent<cbit::ecs::ColliderComponent>();
                    collider.size = sprite.size;
                    collider.isStatic = true;
                }
            }
        }
    }
}

} // namespace

/**
 * @brief Initializes the play scene.
 */
void PlayScene::initialize()
{
    const auto map = cbit2d::assets::TileMap::loadFromFile("apps/sandbox/assets/maps/basic.json");
    if (map) {
        spawnMapTiles(world, *map);
    } else {
        cbit2d::core::Logger::warn("PlayScene is starting without a loaded tile map");
    }

    auto player = world.addGameObject("Player");
    auto& transform = player.getComponent<cbit::ecs::TransformComponent>();
    transform.position = {400.0F, 300.0F};

    auto& sprite = player.addComponent<cbit::ecs::SpriteComponent>();
    sprite.assetPath = "apps/sandbox/assets/images/sokoban_tilesheet.png";
    sprite.textureSize = {832.0F, 512.0F};
    sprite.sourcePosition = {0.0F, 256.0F};
    sprite.sourceSize = {64.0F, 64.0F};
    sprite.size = {64.0F, 64.0F};
    sprite.renderOrder = 2;

    auto& animation = player.addComponent<cbit::ecs::SpriteAnimationComponent>();
    animation.clips["idle"] = {{0.0F, 256.0F}, 3, 0.22F};
    animation.clips["down"] = {{0.0F, 0.0F}, 3, 0.14F};
    animation.clips["up"] = {{0.0F, 64.0F}, 3, 0.14F};
    animation.clips["left"] = {{0.0F, 128.0F}, 3, 0.14F};
    animation.clips["right"] = {{0.0F, 192.0F}, 3, 0.14F};
    animation.currentClipName = "idle";

    auto& collider = player.addComponent<cbit::ecs::ColliderComponent>();
    collider.size = {48.0F, 48.0F};

    const cbit::ecs::GameObjectId playerId = player.getComponent<cbit::ecs::IdComponent>().id;
    world.addSystem([this, playerId](cbit::ecs::EntityComponentSystem& ecs) {
        constexpr float movementSpeed = 180.0F;
        const float frameMovement = movementSpeed * _deltaTimeSeconds;

        auto gameObject = ecs.getGameObject(playerId);
        if (!gameObject) {
            return;
        }

        auto& objectTransform = gameObject.getComponent<cbit::ecs::TransformComponent>();
        auto& objectAnimation = gameObject.getComponent<cbit::ecs::SpriteAnimationComponent>();

        glm::vec2 movementDirection {0.0F, 0.0F};

        if (cbit2d::core::Input::isKeyHeld(cbit2d::core::Key::W)) {
            movementDirection.y -= 1.0F;
        }
        if (cbit2d::core::Input::isKeyHeld(cbit2d::core::Key::S)) {
            movementDirection.y += 1.0F;
        }
        if (cbit2d::core::Input::isKeyHeld(cbit2d::core::Key::A)) {
            movementDirection.x -= 1.0F;
        }
        if (cbit2d::core::Input::isKeyHeld(cbit2d::core::Key::D)) {
            movementDirection.x += 1.0F;
        }

        if (movementDirection != glm::vec2 {0.0F, 0.0F}) {
            const glm::vec2 horizontalTargetPosition {
                objectTransform.position.x + (movementDirection.x * frameMovement),
                objectTransform.position.y
            };
            if (movementDirection.x == 0.0F
                || !collidesWithStaticWorld(ecs, gameObject.getHandle(), horizontalTargetPosition)) {
                objectTransform.position.x = horizontalTargetPosition.x;
            }

            const glm::vec2 verticalTargetPosition {
                objectTransform.position.x,
                objectTransform.position.y + (movementDirection.y * frameMovement)
            };
            if (movementDirection.y == 0.0F
                || !collidesWithStaticWorld(ecs, gameObject.getHandle(), verticalTargetPosition)) {
                objectTransform.position.y = verticalTargetPosition.y;
            }

            std::string nextClipName = "down";
            if (movementDirection.x < 0.0F) {
                nextClipName = "left";
            } else if (movementDirection.x > 0.0F) {
                nextClipName = "right";
            } else if (movementDirection.y < 0.0F) {
                nextClipName = "up";
            } else if (movementDirection.y > 0.0F) {
                nextClipName = "down";
            }

            objectAnimation.currentClipName = nextClipName;
        } else {
            objectAnimation.currentClipName = "idle";
        }
    });

    cbit2d::core::Logger::info("PlayScene initialized");
}

/**
 * @brief Updates the play scene once per frame.
 * @param deltaTimeSeconds Elapsed time since the previous frame in seconds.
 */
void PlayScene::update(float deltaTimeSeconds)
{
    _deltaTimeSeconds = deltaTimeSeconds;
    world.update(deltaTimeSeconds);
}

} // namespace sandbox::scenes
