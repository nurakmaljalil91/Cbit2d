/**
 * @file    tile_map.hpp
 * @brief   Header file for the TileMap class.
 * @details This file contains the definition of reusable tile map data and
 *          loading helpers for importing Tiled JSON maps into the engine.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace cbit2d::assets {

/**
 * @brief Stores one tileset referenced by a tile map.
 */
struct TileMapTileset {
    std::uint32_t firstGid = 0;
    std::uint32_t columns = 0;
    std::uint32_t imageWidth = 0;
    std::uint32_t imageHeight = 0;
    std::uint32_t tileCount = 0;
    std::uint32_t tileWidth = 0;
    std::uint32_t tileHeight = 0;
    std::string name;
    std::string imagePath;
};

/**
 * @brief Stores one tile layer from a tile map.
 */
struct TileMapLayer {
    std::string name;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    bool visible = true;
    std::vector<std::uint32_t> data;
};

/**
 * @brief Stores a loaded tile map and its referenced tilesets.
 */
class TileMap {
public:
    /**
     * @brief Loads a Tiled JSON tile map from disk.
     * @param filePath Path to the Tiled JSON file.
     * @return Loaded tile map when parsing succeeds, otherwise `std::nullopt`.
     */
    static std::optional<TileMap> loadFromFile(const std::filesystem::path& filePath);

    /**
     * @brief Returns the map source file path.
     * @return Path used to load the tile map.
     */
    const std::filesystem::path& getSourcePath() const;

    /**
     * @brief Returns the map width in tiles.
     * @return Tile column count.
     */
    std::uint32_t getWidth() const;

    /**
     * @brief Returns the map height in tiles.
     * @return Tile row count.
     */
    std::uint32_t getHeight() const;

    /**
     * @brief Returns the tile width in pixels.
     * @return Tile width in pixels.
     */
    std::uint32_t getTileWidth() const;

    /**
     * @brief Returns the tile height in pixels.
     * @return Tile height in pixels.
     */
    std::uint32_t getTileHeight() const;

    /**
     * @brief Returns the loaded tile layers.
     * @return Const reference to the layer list.
     */
    const std::vector<TileMapLayer>& getLayers() const;

    /**
     * @brief Returns the loaded tilesets.
     * @return Const reference to the tileset list.
     */
    const std::vector<TileMapTileset>& getTilesets() const;

    /**
     * @brief Returns the tileset that owns the given global tile identifier.
     * @param gid Global tile identifier from the map.
     * @return Pointer to the owning tileset, or `nullptr` when not found.
     */
    const TileMapTileset* findTilesetForGid(std::uint32_t gid) const;

    /**
     * @brief Resolves a tileset image path against the map file location.
     * @param tileset Tileset whose image path should be resolved.
     * @return Absolute or relative path candidate for the tileset image.
     */
    std::filesystem::path resolveTilesetImagePath(const TileMapTileset& tileset) const;

private:
    std::filesystem::path _sourcePath;
    std::uint32_t _width = 0;
    std::uint32_t _height = 0;
    std::uint32_t _tileWidth = 0;
    std::uint32_t _tileHeight = 0;
    std::vector<TileMapLayer> _layers;
    std::vector<TileMapTileset> _tilesets;
};

} // namespace cbit2d::assets
