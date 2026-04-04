/**
 * @file    tile_map.cpp
 * @brief   Source file for the TileMap class.
 * @details This file contains the implementation of reusable tile map loading
 *          helpers for importing Tiled JSON maps into engine-owned data.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-04
 */

#include "cbit/assets/tile_map.hpp"

#include <algorithm>
#include <fstream>
#include <optional>
#include <sstream>

#include "cbit/core/logger.hpp"

namespace cbit2d::assets {

namespace {

/**
 * @brief Reads an entire text file into memory.
 * @param filePath Path to the file to read.
 * @return File contents when reading succeeds, otherwise `std::nullopt`.
 */
std::optional<std::string> readTextFile(const std::filesystem::path& filePath)
{
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        return std::nullopt;
    }

    std::ostringstream outputStream;
    outputStream << fileStream.rdbuf();
    return outputStream.str();
}

/**
 * @brief Finds the position immediately after a named JSON field key.
 * @param content JSON document content.
 * @param fieldName Field name to read.
 * @return Position after the key token when found, otherwise `std::nullopt`.
 */
std::optional<std::size_t> findFieldPosition(const std::string& content, const std::string& fieldName)
{
    const std::string token = "\"" + fieldName + "\"";
    const std::size_t tokenPosition = content.find(token);
    if (tokenPosition == std::string::npos) {
        return std::nullopt;
    }

    return tokenPosition + token.size();
}

/**
 * @brief Skips JSON whitespace and separators from the provided position.
 * @param content JSON document content.
 * @param position Position to advance.
 * @return First content position after whitespace and separators.
 */
std::size_t skipFieldPreamble(const std::string& content, std::size_t position)
{
    while (position < content.size()
        && (content[position] == ' '
            || content[position] == '\t'
            || content[position] == '\n'
            || content[position] == '\r'
            || content[position] == ':')) {
        ++position;
    }

    return position;
}

/**
 * @brief Unescapes a minimal JSON string payload.
 * @param value Escaped JSON string payload.
 * @return Unescaped text string.
 */
std::string unescapeJsonString(const std::string_view value)
{
    std::string result;
    result.reserve(value.size());

    for (std::size_t index = 0; index < value.size(); ++index) {
        if (value[index] == '\\' && index + 1 < value.size()) {
            ++index;
            switch (value[index]) {
            case '\\':
                result.push_back('\\');
                break;
            case '"':
                result.push_back('"');
                break;
            case '/':
                result.push_back('/');
                break;
            case 'n':
                result.push_back('\n');
                break;
            case 'r':
                result.push_back('\r');
                break;
            case 't':
                result.push_back('\t');
                break;
            default:
                result.push_back(value[index]);
                break;
            }
        } else {
            result.push_back(value[index]);
        }
    }

    return result;
}

/**
 * @brief Reads an unsigned integer field from a JSON object string.
 * @param content JSON object content.
 * @param fieldName Field name to read.
 * @param value Output value receiving the parsed integer.
 * @return `true` when the field exists and was parsed successfully.
 */
bool tryReadUnsigned(const std::string& content, const std::string& fieldName, std::uint32_t& value)
{
    const auto fieldPosition = findFieldPosition(content, fieldName);
    if (!fieldPosition) {
        return false;
    }

    std::size_t position = skipFieldPreamble(content, *fieldPosition);
    std::size_t endPosition = position;
    while (endPosition < content.size() && std::isdigit(static_cast<unsigned char>(content[endPosition])) != 0) {
        ++endPosition;
    }

    if (position == endPosition) {
        return false;
    }

    value = static_cast<std::uint32_t>(std::stoul(content.substr(position, endPosition - position)));
    return true;
}

/**
 * @brief Reads a string field from a JSON object string.
 * @param content JSON object content.
 * @param fieldName Field name to read.
 * @param value Output value receiving the parsed string.
 * @return `true` when the field exists and was parsed successfully.
 */
bool tryReadString(const std::string& content, const std::string& fieldName, std::string& value)
{
    const auto fieldPosition = findFieldPosition(content, fieldName);
    if (!fieldPosition) {
        return false;
    }

    std::size_t position = skipFieldPreamble(content, *fieldPosition);
    if (position >= content.size() || content[position] != '"') {
        return false;
    }

    ++position;
    std::size_t endPosition = position;
    bool escaped = false;
    while (endPosition < content.size()) {
        const char currentCharacter = content[endPosition];
        if (!escaped && currentCharacter == '"') {
            break;
        }

        escaped = !escaped && currentCharacter == '\\';
        if (currentCharacter != '\\') {
            escaped = false;
        }
        ++endPosition;
    }

    if (endPosition >= content.size()) {
        return false;
    }

    value = unescapeJsonString(std::string_view(content).substr(position, endPosition - position));
    return true;
}

/**
 * @brief Reads a boolean field from a JSON object string.
 * @param content JSON object content.
 * @param fieldName Field name to read.
 * @param value Output value receiving the parsed boolean.
 * @return `true` when the field exists and was parsed successfully.
 */
bool tryReadBool(const std::string& content, const std::string& fieldName, bool& value)
{
    const auto fieldPosition = findFieldPosition(content, fieldName);
    if (!fieldPosition) {
        return false;
    }

    const std::size_t position = skipFieldPreamble(content, *fieldPosition);
    if (content.compare(position, 4, "true") == 0) {
        value = true;
        return true;
    }

    if (content.compare(position, 5, "false") == 0) {
        value = false;
        return true;
    }

    return false;
}

/**
 * @brief Finds the content inside a named top-level array field.
 * @param content JSON document content.
 * @param fieldName Array field name to read.
 * @return Array inner content when found, otherwise `std::nullopt`.
 */
std::optional<std::string> tryReadArrayContent(const std::string& content, const std::string& fieldName)
{
    const auto fieldPosition = findFieldPosition(content, fieldName);
    if (!fieldPosition) {
        return std::nullopt;
    }

    std::size_t position = skipFieldPreamble(content, *fieldPosition);
    if (position >= content.size() || content[position] != '[') {
        return std::nullopt;
    }

    const std::size_t arrayStart = position + 1;
    int depth = 1;
    ++position;

    while (position < content.size() && depth > 0) {
        if (content[position] == '[') {
            ++depth;
        } else if (content[position] == ']') {
            --depth;
        }

        ++position;
    }

    if (depth != 0 || position == 0) {
        return std::nullopt;
    }

    return content.substr(arrayStart, position - arrayStart - 1);
}

/**
 * @brief Splits a JSON array of objects into per-object strings.
 * @param arrayContent Inner array content without surrounding brackets.
 * @return Vector of serialized JSON object strings.
 */
std::vector<std::string> splitObjects(const std::string& arrayContent)
{
    std::vector<std::string> objects;
    std::size_t objectStart = std::string::npos;
    int depth = 0;

    for (std::size_t index = 0; index < arrayContent.size(); ++index) {
        if (arrayContent[index] == '{') {
            if (depth == 0) {
                objectStart = index;
            }
            ++depth;
        } else if (arrayContent[index] == '}') {
            --depth;
            if (depth == 0 && objectStart != std::string::npos) {
                objects.push_back(arrayContent.substr(objectStart, index - objectStart + 1));
                objectStart = std::string::npos;
            }
        }
    }

    return objects;
}

/**
 * @brief Reads an array of unsigned integers from a JSON object string.
 * @param content JSON object content.
 * @param fieldName Array field name to read.
 * @return Parsed integer array.
 */
std::vector<std::uint32_t> readUnsignedArray(const std::string& content, const std::string& fieldName)
{
    std::vector<std::uint32_t> values;
    const auto arrayContent = tryReadArrayContent(content, fieldName);
    if (!arrayContent) {
        return values;
    }

    std::size_t position = 0;
    while (position < arrayContent->size()) {
        while (position < arrayContent->size()
            && std::isdigit(static_cast<unsigned char>((*arrayContent)[position])) == 0) {
            ++position;
        }

        std::size_t endPosition = position;
        while (endPosition < arrayContent->size()
            && std::isdigit(static_cast<unsigned char>((*arrayContent)[endPosition])) != 0) {
            ++endPosition;
        }

        if (position != endPosition) {
            values.push_back(static_cast<std::uint32_t>(std::stoul(arrayContent->substr(position, endPosition - position))));
        }

        position = endPosition + 1;
    }

    return values;
}

} // namespace

/**
 * @brief Loads a Tiled JSON tile map from disk.
 * @param filePath Path to the Tiled JSON file.
 * @return Loaded tile map when parsing succeeds, otherwise `std::nullopt`.
 */
std::optional<TileMap> TileMap::loadFromFile(const std::filesystem::path& filePath)
{
    const std::filesystem::path resolvedPath = std::filesystem::exists(filePath)
        ? filePath
        : std::filesystem::path("..") / filePath;

    const auto content = readTextFile(resolvedPath);
    if (!content) {
        cbit2d::core::Logger::error("Couldn't read tile map '{}'", filePath.string());
        return std::nullopt;
    }

    TileMap map;
    map._sourcePath = resolvedPath;

    if (!tryReadUnsigned(*content, "width", map._width)
        || !tryReadUnsigned(*content, "height", map._height)
        || !tryReadUnsigned(*content, "tilewidth", map._tileWidth)
        || !tryReadUnsigned(*content, "tileheight", map._tileHeight)) {
        cbit2d::core::Logger::error("Tile map '{}' is missing required size fields", resolvedPath.string());
        return std::nullopt;
    }

    const auto layerContent = tryReadArrayContent(*content, "layers");
    if (!layerContent) {
        cbit2d::core::Logger::error("Tile map '{}' is missing layer data", resolvedPath.string());
        return std::nullopt;
    }

    for (const auto& layerElement : splitObjects(*layerContent)) {
        std::string layerType;
        if (!tryReadString(layerElement, "type", layerType) || layerType != "tilelayer") {
            continue;
        }

        TileMapLayer layer;
        tryReadString(layerElement, "name", layer.name);
        tryReadUnsigned(layerElement, "width", layer.width);
        tryReadUnsigned(layerElement, "height", layer.height);
        tryReadBool(layerElement, "visible", layer.visible);

        layer.data = readUnsignedArray(layerElement, "data");
        map._layers.push_back(std::move(layer));
    }

    const auto tilesetContent = tryReadArrayContent(*content, "tilesets");
    if (!tilesetContent) {
        cbit2d::core::Logger::error("Tile map '{}' is missing tileset data", resolvedPath.string());
        return std::nullopt;
    }

    for (const auto& tilesetElement : splitObjects(*tilesetContent)) {
        TileMapTileset tileset;
        if (!tryReadUnsigned(tilesetElement, "firstgid", tileset.firstGid)) {
            continue;
        }

        tryReadUnsigned(tilesetElement, "columns", tileset.columns);
        tryReadUnsigned(tilesetElement, "imagewidth", tileset.imageWidth);
        tryReadUnsigned(tilesetElement, "imageheight", tileset.imageHeight);
        tryReadUnsigned(tilesetElement, "tilecount", tileset.tileCount);
        tryReadUnsigned(tilesetElement, "tilewidth", tileset.tileWidth);
        tryReadUnsigned(tilesetElement, "tileheight", tileset.tileHeight);
        tryReadString(tilesetElement, "name", tileset.name);
        tryReadString(tilesetElement, "image", tileset.imagePath);

        map._tilesets.push_back(std::move(tileset));
    }

    std::sort(
        map._tilesets.begin(),
        map._tilesets.end(),
        [](const TileMapTileset& left, const TileMapTileset& right) {
            return left.firstGid < right.firstGid;
        }
    );

    return map;
}

/**
 * @brief Returns the map source file path.
 * @return Path used to load the tile map.
 */
const std::filesystem::path& TileMap::getSourcePath() const
{
    return _sourcePath;
}

/**
 * @brief Returns the map width in tiles.
 * @return Tile column count.
 */
std::uint32_t TileMap::getWidth() const
{
    return _width;
}

/**
 * @brief Returns the map height in tiles.
 * @return Tile row count.
 */
std::uint32_t TileMap::getHeight() const
{
    return _height;
}

/**
 * @brief Returns the tile width in pixels.
 * @return Tile width in pixels.
 */
std::uint32_t TileMap::getTileWidth() const
{
    return _tileWidth;
}

/**
 * @brief Returns the tile height in pixels.
 * @return Tile height in pixels.
 */
std::uint32_t TileMap::getTileHeight() const
{
    return _tileHeight;
}

/**
 * @brief Returns the loaded tile layers.
 * @return Const reference to the layer list.
 */
const std::vector<TileMapLayer>& TileMap::getLayers() const
{
    return _layers;
}

/**
 * @brief Returns the loaded tilesets.
 * @return Const reference to the tileset list.
 */
const std::vector<TileMapTileset>& TileMap::getTilesets() const
{
    return _tilesets;
}

/**
 * @brief Returns the tileset that owns the given global tile identifier.
 * @param gid Global tile identifier from the map.
 * @return Pointer to the owning tileset, or `nullptr` when not found.
 */
const TileMapTileset* TileMap::findTilesetForGid(const std::uint32_t gid) const
{
    const TileMapTileset* resolvedTileset = nullptr;
    for (const auto& tileset : _tilesets) {
        if (gid >= tileset.firstGid) {
            resolvedTileset = &tileset;
            continue;
        }

        break;
    }

    return resolvedTileset;
}

/**
 * @brief Resolves a tileset image path against the map file location.
 * @param tileset Tileset whose image path should be resolved.
 * @return Absolute or relative path candidate for the tileset image.
 */
std::filesystem::path TileMap::resolveTilesetImagePath(const TileMapTileset& tileset) const
{
    const std::filesystem::path imagePath(tileset.imagePath);
    if (imagePath.is_absolute()) {
        return imagePath;
    }

    return _sourcePath.parent_path() / imagePath;
}

} // namespace cbit2d::assets
