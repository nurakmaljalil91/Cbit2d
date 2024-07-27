//
// Created by User on 28/7/2024.
//

#include "TileMap.h"

TileMap::TileMap() = default;

TileMap::~TileMap() = default;

void TileMap::cleanup() {
    SDL_DestroyTexture(_tileSetTexture);
}

simdjson::ondemand::document TileMap::loadJSON(const std::string &filePath, simdjson::ondemand::parser &parser) {
    simdjson::padded_string json = simdjson::padded_string::load(filePath);
    return parser.iterate(json);
}

void TileMap::loadMap(std::string textureName, std::string jsonFile, int tileWidth, int tileHeight, entt::registry &registry) {
    _tileSetTexture = AssetManager::getInstance().loadTexture(textureName);

    try {
        simdjson::ondemand::parser parser;
        simdjson::ondemand::document doc = loadJSON(jsonFile, parser);

        int64_t height64 = doc["height"].get_int64().value();
        int64_t width64 = doc["width"].get_int64().value();

        int height = static_cast<int>(height64);
        int width = static_cast<int>(width64);

        // Extract layers
        simdjson::ondemand::array layers = doc["layers"].get_array().value();

        // Iterate through layers
        for (auto layer: layers) {
            std::string name = std::string(layer["name"].get_string().value());
            std::string type = std::string(layer["type"].get_string().value());
            if (type == "tilelayer") {
                simdjson::ondemand::array data = layer["data"].get_array().value();
                int index = 0;
                for (auto tile: data) {
                    int gid = tile.get_int64().value();
                    if (gid != 0) {
                        Tile newTile;
                        newTile.gid = gid;
                        newTile.srcRect = {
                                ((gid - 1) % width) * tileWidth,
                                ((gid - 1) / width) * tileHeight,
                                tileWidth, tileHeight};
                        newTile.destRect = {
                                (index % width) * tileWidth,
                                (index / width) * tileHeight,
                                tileWidth, tileHeight};
                        _tiles.push_back(newTile);
                    }
                    index++;
                }
            }
        }

    } catch (simdjson::simdjson_error &e) {
        spdlog::error("Error parsing JSON: {}", e.what());
    }

}

void TileMap::render(SDL_Renderer *renderer) {
    for (auto &tile: _tiles) {
        SDL_RenderCopy(renderer, _tileSetTexture, &tile.srcRect, &tile.destRect);
    }
}
