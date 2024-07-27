/**
 * @file TileMap.h
 * @brief Header file for the TileMap class.
 *
 * This file contains the definition of the TileMap class which is responsible for managing the tile map in the game.
 * The TileMap class is responsible for loading, storing, and rendering the tile map in the game.
 *
 * @author Nur Akmal bin Jalil
 * @date 2024-07-28
 */

#ifndef CBIT2D_TILEMAP_H
#define CBIT2D_TILEMAP_H

#include <simdjson.h>
#include <SDL2/SDL.h>
#include "AssetManager.h"
#include "../utilities/Logger.h"
#include <entt/entt.hpp>

struct Tile {
    int gid;
    SDL_Rect srcRect;
    SDL_Rect destRect;
};

class TileMap {
public:
    TileMap();

    ~TileMap();

    simdjson::ondemand::document loadJSON(const std::string &filePath, simdjson::ondemand::parser &parser);

    void
    loadMap(std::string textureName, std::string jsonFile, int tileWidth, int tileHeight, entt::registry &registry);

    void render(SDL_Renderer *renderer);

    void cleanup();

private:
    std::vector<Tile> _tiles;
    SDL_Texture *_tileSetTexture{};
};


#endif //CBIT2D_TILEMAP_H
